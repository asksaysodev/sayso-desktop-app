#include "aec_pipeline.h"

#include <algorithm>
#include <chrono>
#include <climits>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <system_error>

namespace sayso::aec {

namespace {

constexpr double kHostTicksPerSecond = 1e7;  // host times are in 100 ns units

// Ring sizes: about a second of audio at up to 192 kHz per path. The worker
// drains every few ms, so these only fill if the worker stalls.
constexpr size_t kSampleRingCapacity = 1 << 18;
constexpr size_t kMetaRingCapacity = 1024;

// Packet timestamps (and arrival times, for streams placed by arrival) jitter
// around a stream's true timing: by tens of ms on virtual and remote devices,
// with occasional bursts of 100 ms or more. Placement acts on a smoothed
// error over about the last 128 packets (~1.3 s at 10 ms packets), and each
// packet's pull on it is capped, so one late burst cannot swing it.
constexpr double kErrorSmoothing = 1.0 / 128;
constexpr double kErrorClampSamples = AecPipeline::kProcessRate * 30 / 1000.0;  // 30 ms
// Clock drift between mic and system audio is corrected continuously by
// trimming the system audio's conversion ratio, so its timing error is worked
// off over about this many seconds, within +/-1000 ppm (real clocks differ by
// tens of ppm). Slow on purpose: drift is a near-constant offset, and a fast
// correction would follow packet jitter and make the reference waver, which
// blurs the engine's echo model. At 100 ppm the residual error is ~2 ms, well
// within the engine's own delay tracking.
constexpr double kDriftCorrectionSeconds = 20.0;
constexpr double kMaxRatioTrim = 1e-3;
constexpr double kTrimDeadbandSamples = AecPipeline::kProcessRate * 1.5 / 1000.0;  // 1.5 ms
// An error larger than this (a slip trimming would take too long to absorb)
// is stepped out at once instead.
constexpr double kSlipSamples = AecPipeline::kProcessRate * 20 / 1000.0;  // 20 ms
// A reference packet this much earlier than its stream's own trend means
// that stream broke (it ran backwards); adapted filter state no longer
// matches and the engine starts over. Late packets are gaps, never breaks.
constexpr double kResetSamples = AecPipeline::kProcessRate / 10.0;  // 100 ms
// A reference packet arriving this much later than its sample count predicts
// follows a stretch the device never sent (silence), not a break: at least
// 7 ms (below a single missing 10 ms packet; real loopback timestamps jitter
// well under 1 ms), or 3x the stream's mean early-packet wander if larger
// (1.5x the worst case of evenly spread jitter), so noisy timing is not
// mistaken for missing packets.
constexpr double kReferenceGapSamples = AecPipeline::kProcessRate * 7 / 1000.0;  // 7 ms
constexpr double kGapJitterFactor = 3.0;
constexpr double kJitterSmoothing = 1.0 / 64;
constexpr double kJitterCapSamples = AecPipeline::kProcessRate * 50 / 1000.0;  // 50 ms
// The mic is the master clock: its audio is re-placed (and the engine
// restarted) only this far off its trend, a genuine break rather than a
// burst of late packets.
constexpr double kCaptureBreakSamples = AecPipeline::kProcessRate * 2.0;  // 2 s
// A gap longer than this is not filled with silence; the stream restarts
// from the new position instead.
constexpr int64_t kMaxGapFillSamples = AecPipeline::kProcessRate * 2;
// How long a mic frame may wait for its system-audio reference before the
// missing part is treated as silence. Covers loopback packet jitter; system
// audio that is not playing anything may send no packets at all.
constexpr uint64_t kReferenceWaitTicks = 25 * 10000;  // 25 ms
// System audio counts as quiet (nothing to wait for) once no packet has
// arrived for this long; it delivers a packet about every 10 ms while playing.
constexpr uint64_t kReferenceIdleTicks = 40 * 10000;  // 40 ms
// Unconsumed audio is bounded so a stalled path cannot grow memory.
constexpr size_t kMaxBacklogSamples = AecPipeline::kProcessRate * 2;
// After this many failed frames in a row the engine is dropped and the mic
// passes through until the next reset.
constexpr int kMaxConsecutiveErrors = 100;
constexpr uint64_t kReferenceGraceFrames = 100;  // 1 s of mic frames
// A packet timestamp further than this from the packet's arrival is not on
// the shared host clock. Real devices stamp within tens of ms of arrival.
constexpr int64_t kTimestampTrustTicks = 10000000;  // 1 s
// A stream's clock may jump (a laptop sleep) once its timestamps have been on
// the shared clock for this many packets (about a second); a stream that is
// off from the start, or keeps jumping, is placed by arrival instead. A jump
// is confirmed by this many packets in a row off by the same amount.
constexpr uint64_t kClockJumpMinPackets = 100;
constexpr uint64_t kClockJumpMinIntervalTicks = 5 * 10000000ull;  // 5 s
constexpr int kClockJumpConfirmPackets = 20;
constexpr int64_t kClockJumpToleranceTicks = 1000000;  // 100 ms
constexpr uint64_t kStatsIntervalTicks = 60ull * 10000000ull;  // 60 s

std::string Fmt(const char* format, ...) {
  char buf[512];
  va_list args;
  va_start(args, format);
  vsnprintf(buf, sizeof(buf), format, args);
  va_end(args);
  return buf;
}

// Original bytes of mic packets the pipeline delivers: ~2.7 s of 48 kHz
// stereo float, far more than the worker ever holds.
constexpr size_t kRawRingCapacity = 1 << 21;

int TypeBytes(SampleType type) {
  switch (type) {
    case SampleType::kInt16: return 2;
    case SampleType::kInt24: return 3;
    case SampleType::kFloat32:
    case SampleType::kInt32: return 4;
    default: return 0;
  }
}

}  // namespace

AecPipeline::Input::Input() : samples(kSampleRingCapacity), meta(kMetaRingCapacity), raw(kRawRingCapacity) {}

AecPipeline::AecPipeline(EmitFn emit, HostClockFn now) : emit_(emit), now_(now) {
  captureTrack_.name = "mic";
  renderTrack_.name = "system";
  captureFrame_.resize(kFrameSamples);
  renderFrame_.resize(kFrameSamples);
  cleanFrame_.resize(kFrameSamples);
}

AecPipeline::~AecPipeline() { Stop(); }

void AecPipeline::SetCleanSink(CleanSink sink) {
  if (!running_.load(std::memory_order_acquire)) {
    sink_ = std::move(sink);
  }
}

void AecPipeline::SetDiagnosticRecording(const std::string& folderUtf8, double maxSeconds) {
  if (!running_.load(std::memory_order_acquire)) {
    recordFolder_ = folderUtf8;
    recordMaxSeconds_ = maxSeconds;
  }
}

void AecPipeline::SetDeliverySink(DeliverFn deliver) {
  if (!running_.load(std::memory_order_acquire)) {
    deliver_ = std::move(deliver);
  }
}

void AecPipeline::SetEnabled(bool on) {
  enabled_.store(on, std::memory_order_release);
  if (on && deliver_ && running_.load(std::memory_order_acquire)) {
    route_.fetch_or(kRouteBit, std::memory_order_acq_rel);  // from the next mic packet
  }
  // Switching off: the worker hands back what it holds, then clears the bit.
}

AecPipeline::State AecPipeline::GetState() const {
  State st;
  st.enabled = enabled_.load(std::memory_order_acquire);
  st.active = activeState_.load(std::memory_order_acquire);
  st.delayMs = delayState_.load(std::memory_order_acquire);
  return st;
}

void AecPipeline::Start() {
  if (worker_.joinable()) {
    return;
  }
  // No worker is running, so this thread may act as the rings' consumer and
  // discard whatever an earlier session left behind.
  for (Input* in : {&capture_, &render_}) {
    in->samples.Clear();
    in->meta.Clear();
    in->raw.Clear();
    in->dropped.store(0, std::memory_order_relaxed);
  }
  for (Track* t : {&captureTrack_, &renderTrack_}) {
    const char* name = t->name;
    *t = Track();
    t->name = name;
  }
  haveOrigin_ = false;
  captureSkew_ = 0;
  engine_.reset();
  engineUnavailable_ = false;
  engineFailed_ = false;
  pendingReset_.clear();
  consecutiveErrors_ = 0;
  referenceState_ = -1;
  captureFrames_ = aecFrames_ = passthroughFrames_ = referenceMissingSamples_ = 0;
  engineErrors_ = resets_ = lateFramesDropped_ = uncoveredSamples_ = 0;
  lateFramesReported_ = routeFallbacksReported_ = lastTroubleHost_ = 0;
  renderLastArrival_ = 0;
  referenceLateFrames_ = latencySumTicks_ = latencyMaxTicks_ = latencyFrames_ = 0;
  processSumUs_ = processMaxUs_ = 0;
  periodLevels_ = Levels();
  sessionLevels_ = Levels();
  lastStatsHost_ = now_();

  // Opened before the worker exists: from then on only the worker touches it.
  std::string recordingEvent;
  if (!recordFolder_.empty()) {
#ifdef SAYSO_AEC_DIAG_RECORDING
    char stem[64];
    const std::time_t t = std::time(nullptr);
    std::strftime(stem, sizeof(stem), "sayso-aec-%Y%m%dT%H%M%SZ", std::gmtime(&t));
    std::string error;
    recordingEvent = recorder_.Open(recordFolder_, stem, kProcessRate, recordMaxSeconds_, &error)
                         ? Fmt("aec_recording_started files=%s-{reference,mic_raw,mic_clean}.wav max_s=%.0f",
                               stem, recordMaxSeconds_)
                         : "aec_recording_failed reason=" + error;
#else
    // Call recording is compiled into developer builds only.
    recordingEvent = "aec_recording_unavailable reason=not_built";
#endif
  }

  {
    std::lock_guard<std::mutex> lk(stopMutex_);
    stopRequested_ = false;
  }
  activeState_.store(false, std::memory_order_release);
  delayState_.store(-1, std::memory_order_release);
  routeFallbacks_.store(0, std::memory_order_relaxed);
  route_.store(enabled_.load(std::memory_order_acquire) && deliver_ ? kRouteBit : 0, std::memory_order_release);
  running_.store(true, std::memory_order_release);
  try {
    worker_ = std::thread([this] { WorkerMain(); });
  } catch (const std::system_error& e) {
    // No worker, no echo cancellation: the mic keeps its normal path.
    running_.store(false, std::memory_order_release);
    route_.store(0, std::memory_order_release);
    emit_(Fmt("aec_failed reason=worker_start_failed code=%d", e.code().value()));
    return;
  }
  emit_(Fmt("aec_started rate=%d frame_ms=10", kProcessRate));
  if (!recordingEvent.empty()) {
    emit_(recordingEvent);
  }
}

void AecPipeline::Stop() {
  if (!worker_.joinable()) {
    return;
  }
  running_.store(false, std::memory_order_release);
  {
    std::lock_guard<std::mutex> lk(stopMutex_);
    stopRequested_ = true;
  }
  stopCv_.notify_all();
  worker_.join();
  // The mic is stopping: like the capture path, whatever was still in flight
  // is dropped. Capture threads now deliver directly again.
  route_.store(0, std::memory_order_release);
  activeState_.store(false, std::memory_order_release);
}

// ── Producer side ─────────────────────────────────────────────────────────

bool AecPipeline::PushPacket(AecPath path, uint64_t streamId, double sampleRate, const uint8_t* data, uint32_t frames,
                             int channels, SampleType type, uint64_t hostTime, bool hostTimeValid) {
  if (!running_.load(std::memory_order_acquire) || frames == 0 || sampleRate <= 0) {
    return false;
  }
  Input& in = InputFor(path);
  const size_t rawBytes = static_cast<size_t>(frames) * channels * TypeBytes(type);
  if (in.samples.FreeSpace() < frames || in.meta.FreeSpace() < 1) {
    in.dropped.fetch_add(1, std::memory_order_relaxed);
    if (path == AecPath::kCapture && (route_.load(std::memory_order_acquire) & kRouteBit)) {
      routeFallbacks_.fetch_add(1, std::memory_order_relaxed);  // the caller delivers it
    }
    return false;
  }
  // Take over delivering this mic packet while echo cancellation is on: count
  // it in, but only while the route bit is still set.
  bool takeOver = false;
  if (path == AecPath::kCapture && rawBytes > 0) {
    uint64_t route = route_.load(std::memory_order_acquire);
    while (route & kRouteBit) {
      if (in.raw.FreeSpace() < rawBytes) {
        routeFallbacks_.fetch_add(1, std::memory_order_relaxed);
        break;
      }
      if (route_.compare_exchange_weak(route, route + 1, std::memory_order_acq_rel)) {
        takeOver = true;
        break;
      }
    }
  }
  if (in.mono.size() < frames) {
    in.mono.resize(frames);  // grows to the device's packet size once, then never again
  }
  DownmixToMono(data, frames, channels, type, in.mono.data());
  PacketMeta meta;
  meta.kind = PacketMeta::kData;
  meta.frames = frames;
  meta.streamId = streamId;
  meta.hostTime = hostTime;
  meta.hostTimeValid = hostTimeValid;
  meta.arrivalHost = now_();
  meta.sampleRate = sampleRate;
  meta.samplePos = in.samples.WritePosition();
  meta.deliver = takeOver;
  meta.channels = static_cast<uint8_t>(channels);
  meta.type = type;
  if (takeOver) {
    meta.rawPos = in.raw.WritePosition();
    if (data) {
      in.raw.Write(data, rawBytes);
    } else {
      if (in.zeros.size() < rawBytes) in.zeros.resize(rawBytes, 0);  // a packet flagged silent is delivered as zeros
      in.raw.Write(in.zeros.data(), rawBytes);
    }
  }
  in.samples.Write(in.mono.data(), frames);
  in.meta.Write(&meta, 1);
  return takeOver;
}

void AecPipeline::EndStream(AecPath path, uint64_t streamId) {
  if (!running_.load(std::memory_order_acquire)) {
    return;
  }
  Input& in = InputFor(path);
  PacketMeta meta;
  meta.kind = PacketMeta::kEnd;
  meta.streamId = streamId;
  meta.samplePos = in.samples.WritePosition();
  if (!in.meta.Write(&meta, 1)) {
    in.dropped.fetch_add(1, std::memory_order_relaxed);
  }
}

// ── Worker ────────────────────────────────────────────────────────────────

void AecPipeline::WorkerMain() {
  for (;;) {
    {
      std::unique_lock<std::mutex> lk(stopMutex_);
      if (stopCv_.wait_for(lk, std::chrono::milliseconds(5), [this] { return stopRequested_; })) {
        break;
      }
    }
    DrainInput(AecPath::kCapture);
    DrainInput(AecPath::kRender);
    ProcessFrames();
    UpdateRouting();
    ReportDeliveryTrouble();
    const uint64_t now = now_();
    if (now - lastStatsHost_ >= kStatsIntervalTicks) {
      lastStatsHost_ = now;
      EmitStats("aec_stats");
    }
  }
  EmitStats("aec_stopped");
#ifdef SAYSO_AEC_DIAG_RECORDING
  if (recorder_.IsOpen()) {
    recorder_.Close();
    emit_(Fmt("aec_recording_stopped files=%s seconds=%.1f dropped_blocks=%llu", recorder_.Stem().c_str(),
              recorder_.SecondsWritten(), static_cast<unsigned long long>(recorder_.DroppedBlocks())));
  }
#endif
  engine_.reset();
}

void AecPipeline::DrainInput(AecPath path) {
  Input& in = InputFor(path);
  Track& track = TrackFor(path);
  while (in.meta.Available() > 0) {
    PacketMeta meta;
    in.meta.Read(&meta, 1);
    if (path == AecPath::kRender && meta.kind == PacketMeta::kData) {
      renderLastArrival_ = std::max(renderLastArrival_, meta.arrivalHost);
    }
    // A Clear() in Start() can race a producer between its sample write and
    // its meta write; the recorded position tells which samples are whose.
    // A packet the pipeline took over carries its original bytes in the raw
    // ring; collect them first, so it is handed over even if its samples
    // cannot be placed.
    if (meta.kind == PacketMeta::kData && meta.deliver) {
      const size_t rawBytes = static_cast<size_t>(meta.frames) * meta.channels * TypeBytes(meta.type);
      const uint64_t rawRead = in.raw.ReadPosition();
      if (meta.rawPos >= rawRead) {
        in.raw.Skip(std::min<uint64_t>(meta.rawPos - rawRead, in.raw.Available()));
      }
      track.packetRaw.resize(rawBytes);
      if (meta.rawPos >= rawRead && in.raw.Available() >= rawBytes) {
        in.raw.Read(track.packetRaw.data(), rawBytes);
      } else {
        std::fill(track.packetRaw.begin(), track.packetRaw.end(), 0);  // not expected: rings are never cleared while running
      }
    }
    const uint64_t readPos = in.samples.ReadPosition();
    if (meta.samplePos < readPos) {
      if (meta.deliver) route_.fetch_sub(1, std::memory_order_acq_rel);
      continue;  // its samples were discarded
    }
    in.samples.Skip(std::min<uint64_t>(meta.samplePos - readPos, in.samples.Available()));
    if (meta.kind == PacketMeta::kEnd) {
      if (track.active && meta.streamId == track.streamId) {
        EndTrack(track, "stream_ended");
      }
      continue;
    }
    if (in.samples.Available() < meta.frames) {
      if (meta.deliver) route_.fetch_sub(1, std::memory_order_acq_rel);
      continue;  // samples discarded under it; nothing to place
    }
    track.packet.resize(meta.frames);
    in.samples.Read(track.packet.data(), meta.frames);
    HandleData(track, meta);
  }
}

int64_t AecPipeline::TimelineIndex(uint64_t hostTime) const {
  const double seconds = static_cast<double>(static_cast<int64_t>(hostTime - originHost_)) / kHostTicksPerSecond;
  return static_cast<int64_t>(std::llround(seconds * kProcessRate));
}

void AecPipeline::BeginTrack(Track& track, const PacketMeta& meta) {
  // The packet that announced the new stream sits in track.packet, waiting to
  // be resampled; it must survive the reset.
  const char* name = track.name;
  if (&track == &captureTrack_) {
    FlushDelivery();  // hand over whatever the old stream still held, in order
  }
  std::vector<float> packet = std::move(track.packet);
  std::vector<uint8_t> packetRaw = std::move(track.packetRaw);
  track = Track();
  track.name = name;
  track.packet = std::move(packet);
  track.packetRaw = std::move(packetRaw);
  if (&track == &captureTrack_) {
    captureSkew_ = 0;
  }
  track.active = true;
  track.streamId = meta.streamId;
  // The system audio's ratio is trimmed to follow the mic's clock (drift);
  // the mic is the master clock and is never trimmed.
  track.resampler = std::make_unique<Resampler>(meta.sampleRate, kProcessRate, &track == &renderTrack_);
  emit_(Fmt("aec_stream_started path=%s stream=%llu rate=%.0f", name, static_cast<unsigned long long>(meta.streamId),
            meta.sampleRate));
  RequestReset(std::string(name) + "_stream_changed");
}

void AecPipeline::EndTrack(Track& track, const char* why) {
  emit_(Fmt("aec_stream_ended path=%s stream=%llu reason=%s resyncs=%llu gap_samples=%llu", track.name,
            static_cast<unsigned long long>(track.streamId), why, static_cast<unsigned long long>(track.resyncs),
            static_cast<unsigned long long>(track.gapSamples)));
  track.active = false;
  if (&track == &renderTrack_) {
    RequestReset("reference_lost");
  } else {
    FlushDelivery();  // the mic stream ended: hand over what it still held
  }
}

void AecPipeline::HandleData(Track& track, const PacketMeta& meta) {
  if (!track.active || meta.streamId != track.streamId) {
    BeginTrack(track, meta);
  }
  if (meta.deliver) {
    // A mic packet the pipeline hands to the app: keep its original bytes
    // until its stretch of audio has been processed.
    Track::Span span;
    span.begin = track.inSamples;
    span.end = track.inSamples + meta.frames;
    span.bytes = std::move(track.packetRaw);
    span.rate = meta.sampleRate;
    span.channels = meta.channels;
    span.type = meta.type;
    track.deliverSpans.push_back(std::move(span));
    track.packetRaw.clear();
  }
  // Which resampler output this packet's first sample becomes: exact even
  // while the system-audio ratio is being trimmed to follow the mic's clock.
  const double firstOutput = track.resampler->NextInputOutputPosition();
  track.resampled.clear();
  track.resampler->Process(track.packet.data(), meta.frames, track.resampled);

  // Some devices stamp packets on a clock other than the shared host clock:
  // the Remote Desktop mic's timestamps run days away from the system audio's.
  // A timestamp that far from when the packet arrived cannot be on the shared
  // clock, so that stream is placed by arrival time from then on; the
  // smoothing below absorbs the extra jitter.
  //
  // A stream whose timestamps were on the shared clock and then stay far from
  // it by a steady amount had its own clock stopped: some mics stop theirs
  // while the laptop sleeps and carry on from where they were. Their
  // timestamps are still precise, just shifted, so they are shifted back
  // rather than given up for the much rougher arrival time. A short run of
  // far-off packets (audio held over from before the sleep) is placed by its
  // own timestamps and needs nothing more.
  bool useTimestamp = meta.hostTimeValid && !track.timestampsUntrusted;
  uint64_t stamp = meta.hostTime + static_cast<uint64_t>(track.clockShift);
  if (useTimestamp) {
    const int64_t distance = static_cast<int64_t>(meta.arrivalHost - stamp);
    if (distance > kTimestampTrustTicks || distance < -kTimestampTrustTicks) {
      const bool wasTrusted = track.trustedPackets >= kClockJumpMinPackets;
      const bool jumpedRecently =
          track.lastClockJumpHost != 0 && meta.arrivalHost - track.lastClockJumpHost < kClockJumpMinIntervalTicks;
      if (!wasTrusted || jumpedRecently) {
        track.timestampsUntrusted = true;
        track.placed = false;  // placed from those timestamps so far: place it afresh
        useTimestamp = false;
        emit_(Fmt("aec_timestamps_untrusted path=%s offset_s=%.1f placing_by=arrival", track.name,
                  static_cast<double>(distance) / kHostTicksPerSecond));
      } else {
        const int64_t step = distance - track.jumpDistance;
        if (track.jumpPackets == 0 || step > kClockJumpToleranceTicks || step < -kClockJumpToleranceTicks) {
          track.jumpPackets = 0;
          track.jumpDistance = distance;
        }
        if (++track.jumpPackets >= kClockJumpConfirmPackets) {
          const int64_t shift = distance - static_cast<int64_t>(std::llround(track.arrivalLagEma));
          track.clockShift += shift;
          stamp += static_cast<uint64_t>(shift);
          track.lastClockJumpHost = meta.arrivalHost;
          track.jumpPackets = 0;
          emit_(Fmt("aec_clock_jumped path=%s shift_s=%.1f", track.name,
                    static_cast<double>(shift) / kHostTicksPerSecond));
        }
      }
    } else {
      track.jumpPackets = 0;
      track.arrivalLagEma = track.trustedPackets == 0
                                ? static_cast<double>(distance)
                                : track.arrivalLagEma + (static_cast<double>(distance) - track.arrivalLagEma) / 64.0;
      track.trustedPackets++;
    }
  }
  // Arrival marks the end of the packet; its first sample came one packet earlier.
  const uint64_t packetTicks = static_cast<uint64_t>(meta.frames * kHostTicksPerSecond / meta.sampleRate);
  const uint64_t placeAt = useTimestamp ? stamp : meta.arrivalHost - packetTicks;

  {
    if (!haveOrigin_) {
      haveOrigin_ = true;
      originHost_ = placeAt;
    }
    const bool isCapture = &track == &captureTrack_;
    // Where the timestamp says this packet starts, in mic-timeline samples.
    // The reference is mapped through the mic's own smoothed clock error, so
    // it lines up with the mic as heard rather than with raw QPC.
    double measured = static_cast<double>(TimelineIndex(placeAt)) - firstOutput;
    if (!isCapture) {
      measured -= captureSkew_;
    }
    if (!track.placed) {
      track.offset = static_cast<int64_t>(std::llround(measured));
      track.errEma = 0;
      track.placed = true;
    } else {
      const double err = measured - static_cast<double>(track.offset);
      const double deviation = err - track.errEma;
      // How much this stream's timing normally wanders: precise loopback
      // timestamps keep it near zero, arrival-time placement makes it large.
      // Measured from early packets only: silence gaps only ever make packets
      // late, so they cannot inflate it, while jitter is early as often as late.
      if (deviation < 0) {
        track.jitterEma += (std::min(-deviation, kJitterCapSamples) - track.jitterEma) * kJitterSmoothing;
      }
      const double gapThreshold = std::max(kReferenceGapSamples, kGapJitterFactor * track.jitterEma);
      const bool broke = isCapture ? std::fabs(deviation) > kCaptureBreakSamples
                                   : deviation < -kResetSamples;  // late system audio is a gap, never a break
      if (!isCapture && deviation > gapThreshold) {
        // System audio sent nothing for a while (Windows loopback stays quiet
        // while nothing plays), so these samples resume later than counted.
        // Not a break: place them where they belong; the gap becomes silence
        // and the engine keeps what it has learned.
        track.offset += static_cast<int64_t>(std::llround(deviation));
      } else if (broke) {
        // Far off its own trend: the stream broke (lost audio, device
        // hiccup). Re-place it, and start the engine over.
        track.offset = static_cast<int64_t>(std::llround(measured));
        track.errEma = 0;
        track.resyncs++;
        emit_(Fmt("aec_resynced path=%s reason=jump err_ms=%.1f", track.name, err * 1000.0 / kProcessRate));
        RequestReset(std::string(track.name) + "_timing_jump");
      } else {
        // Only the smoothed error is acted on, and no single packet moves it far.
        track.errEma += std::clamp(deviation, -kErrorClampSamples, kErrorClampSamples) * kErrorSmoothing;
        if (isCapture) {
          // The mic's audio is never shifted for jitter or drift; its clock
          // error is carried over to place the reference instead.
          captureSkew_ = track.errEma;
        } else {
          // Follow the mic's clock smoothly: trim the system audio's
          // conversion ratio so its timing error is worked off over a couple
          // of seconds, with no jump the engine would have to re-adapt to.
          // An error too large to trim away quickly is still stepped out.
          if (std::fabs(track.errEma) > kSlipSamples) {
            const int64_t shift = static_cast<int64_t>(std::llround(track.errEma));
            track.offset += shift;
            track.errEma -= static_cast<double>(shift);
            track.resyncs++;
            emit_(Fmt("aec_resynced path=%s reason=slip shift_ms=%.1f", track.name,
                      static_cast<double>(shift) * 1000.0 / kProcessRate));
          }
          // Errors inside the deadband are left alone: the engine tracks
          // them itself, and trimming on them would only follow jitter.
          const double excess = std::copysign(std::max(std::fabs(track.errEma) - kTrimDeadbandSamples, 0.0),
                                              track.errEma);
          track.resampler->SetRatioTrim(
              std::clamp(-excess / (kDriftCorrectionSeconds * kProcessRate), -kMaxRatioTrim, kMaxRatioTrim));
        }
      }
    }
  }

  WriteFifo(track, static_cast<int64_t>(track.outSamples) + track.offset, track.resampled.data(),
            track.resampled.size());
  track.outSamples += track.resampled.size();
  track.inSamples += meta.frames;
}

void AecPipeline::WriteFifo(Track& track, int64_t index, const float* data, size_t n) {
  if (!track.fifoStarted) {
    track.fifoStarted = true;
    track.fifoStart = index;
  }
  int64_t end = track.fifoStart + static_cast<int64_t>(track.fifo.size());
  if (index > end) {
    const int64_t gap = index - end;
    if (gap > kMaxGapFillSamples) {
      track.fifo.clear();
      track.fifoStart = index;
    } else {
      track.fifo.insert(track.fifo.end(), static_cast<size_t>(gap), 0.0f);
      track.gapSamples += static_cast<uint64_t>(gap);
    }
  } else if (index < end) {
    // Overlap after a resync: the newer placement wins.
    const int64_t cut = std::max(index, track.fifoStart);
    track.fifo.resize(static_cast<size_t>(cut - track.fifoStart));
    const int64_t skip = std::min<int64_t>(cut - index, static_cast<int64_t>(n));
    data += skip;
    n -= static_cast<size_t>(skip);
  }
  track.fifo.insert(track.fifo.end(), data, data + n);
  if (track.fifo.size() > kMaxBacklogSamples) {
    const size_t drop = track.fifo.size() - kMaxBacklogSamples;
    track.fifo.erase(track.fifo.begin(), track.fifo.begin() + static_cast<std::ptrdiff_t>(drop));
    track.fifoStart += static_cast<int64_t>(drop);
    if (&track == &captureTrack_) {
      lateFramesDropped_ += drop / kFrameSamples;
    }
  }
}

void AecPipeline::RequestReset(const std::string& reason) {
  if (pendingReset_.empty()) {
    pendingReset_ = reason;
  }
}

void AecPipeline::ApplyPendingReset() {
  if (pendingReset_.empty()) {
    return;
  }
  // Never carry adapted filter state across a stream or route change.
  engine_.reset();
  engineUnavailable_ = false;
  engineFailed_ = false;
  consecutiveErrors_ = 0;
  resets_++;
  if (renderTrack_.active && captureTrack_.active) {
    std::string failReason;
    engine_ = CreateAecEngine(kProcessRate, &failReason);
    if (engine_) {
      emit_(Fmt("aec_engine_ready engine=%s reason=%s", engine_->Name(), pendingReset_.c_str()));
    } else {
      engineUnavailable_ = true;
      // A build without the library is expected to pass the mic through; a
      // library that will not start is a failure the app must hear about.
      emit_(failReason == "not_built" ? "aec_engine_unavailable reason=" + failReason
                                      : "aec_failed reason=engine_" + failReason);
    }
  }
  pendingReset_.clear();
}

void AecPipeline::ProcessFrames() {
  Track& cap = captureTrack_;
  Track& ref = renderTrack_;
  while (cap.fifo.size() >= kFrameSamples) {
    const int64_t frameStart = cap.fifoStart;
    const int64_t frameEnd = frameStart + static_cast<int64_t>(kFrameSamples);

    // Give the reference a chance to arrive before treating it as silence, but
    // only while system audio is actually flowing: when it has gone quiet
    // (Windows sends nothing while nothing plays), waiting would only delay
    // the mic. Every millisecond waited here is latency added to the mic.
    // The frame was captured where the mic's own clock places it: audio the
    // mic lost, or a mic clock running slow, puts it later than its sample
    // count alone says, and the wait must start from then.
    const uint64_t frameEndHost =
        originHost_ + static_cast<uint64_t>(static_cast<int64_t>(std::llround(
                          (static_cast<double>(frameEnd) + captureSkew_) * kHostTicksPerSecond / kProcessRate)));
    const uint64_t now = now_();
    if (ref.active) {
      const int64_t refEnd = ref.fifoStarted ? ref.fifoStart + static_cast<int64_t>(ref.fifo.size()) : INT64_MIN;
      const bool referenceFlowing = now < renderLastArrival_ + kReferenceIdleTicks;
      if (refEnd < frameEnd && referenceFlowing) {
        if (now < frameEndHost + kReferenceWaitTicks) {
          return;
        }
        referenceLateFrames_++;  // gave up waiting while audio was playing
      }
    }
    // Mic latency added by this pipeline: how long after the frame's last
    // sample was captured it is processed.
    if (now > frameEndHost) {
      const uint64_t waited = now - frameEndHost;
      latencySumTicks_ += waited;
      latencyMaxTicks_ = std::max(latencyMaxTicks_, waited);
      latencyFrames_++;
    }
    const auto processStart = std::chrono::steady_clock::now();

    ApplyPendingReset();

    std::copy(cap.fifo.begin(), cap.fifo.begin() + kFrameSamples, captureFrame_.begin());
    cap.fifo.erase(cap.fifo.begin(), cap.fifo.begin() + kFrameSamples);
    cap.fifoStart = frameEnd;
    captureFrames_++;

    // Report the reference appearing or going away. At session start, give
    // system audio a second to begin before calling it missing.
    const int refState = ref.active ? 1 : 0;
    if (refState != referenceState_ && (refState == 1 || referenceState_ == 1 || captureFrames_ >= kReferenceGraceFrames)) {
      emit_(refState == 1                 ? "aec_reference_present"
            : referenceState_ == 1        ? "aec_reference_missing reason=system_audio_stopped"
                                          : "aec_reference_missing reason=system_audio_not_started");
      referenceState_ = refState;
    }

    // The reference slice for exactly this frame's time span, silence where
    // nothing arrived. Reference older than this frame is no longer useful.
    std::fill(renderFrame_.begin(), renderFrame_.end(), 0.0f);
    if (ref.fifoStarted) {
      if (ref.fifoStart < frameStart) {
        const size_t stale = static_cast<size_t>(
            std::min<int64_t>(frameStart - ref.fifoStart, static_cast<int64_t>(ref.fifo.size())));
        ref.fifo.erase(ref.fifo.begin(), ref.fifo.begin() + static_cast<std::ptrdiff_t>(stale));
        ref.fifoStart += static_cast<int64_t>(stale);
        if (ref.fifo.empty()) {
          ref.fifoStart = frameStart;
        }
      }
      if (ref.fifoStart >= frameStart && ref.fifoStart < frameEnd) {
        const size_t at = static_cast<size_t>(ref.fifoStart - frameStart);
        const size_t take = std::min(kFrameSamples - at, ref.fifo.size());
        std::copy(ref.fifo.begin(), ref.fifo.begin() + static_cast<std::ptrdiff_t>(take), renderFrame_.begin() + at);
        ref.fifo.erase(ref.fifo.begin(), ref.fifo.begin() + static_cast<std::ptrdiff_t>(take));
        ref.fifoStart += static_cast<int64_t>(take);
        if (ref.active) {
          referenceMissingSamples_ += kFrameSamples - take;
        }
      } else if (ref.active) {
        referenceMissingSamples_ += kFrameSamples;
      }
    }

    const float* out = captureFrame_.data();
    if (engine_ && ref.active) {
      const int renderErr = engine_->ProcessRender(renderFrame_.data());
      const int captureErr = renderErr == 0 ? engine_->ProcessCapture(captureFrame_.data(), cleanFrame_.data()) : 0;
      if (renderErr == 0 && captureErr == 0) {
        out = cleanFrame_.data();
        aecFrames_++;
        consecutiveErrors_ = 0;
      } else {
        // Never send a broken frame: this one goes out as the raw mic.
        engineErrors_++;
        passthroughFrames_++;
        if (++consecutiveErrors_ >= kMaxConsecutiveErrors) {
          emit_(Fmt("aec_failed reason=process_error code=%d", renderErr != 0 ? renderErr : captureErr));
          engine_.reset();
          engineFailed_ = true;
        }
      }
    } else {
      passthroughFrames_++;
    }
    const uint64_t processUs = static_cast<uint64_t>(
        std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - processStart).count());
    processSumUs_ += processUs;
    processMaxUs_ = std::max(processMaxUs_, processUs);

    for (size_t i = 0; i < kFrameSamples; i++) {
      const double m = captureFrame_[i], c = out[i], r = renderFrame_[i];
      periodLevels_.mic += m * m;
      periodLevels_.clean += c * c;
      periodLevels_.reference += r * r;
    }
    periodLevels_.samples += kFrameSamples;

#ifdef SAYSO_AEC_DIAG_RECORDING
    if (recorder_.IsOpen() && !recorder_.LimitReached()) {
      if (!recorder_.WriteFrame(renderFrame_.data(), captureFrame_.data(), out, kFrameSamples)) {
        emit_(Fmt("aec_recording_limit_reached seconds=%.1f", recorder_.SecondsWritten()));
      }
    }
#endif

    if (sink_) {
      sink_(out, kFrameSamples);
    }
    // To the app: cleaned audio when the engine cancelled this frame and the
    // switch is on; otherwise the original bytes, exactly as captured.
    DeliverFrame(out, out == cleanFrame_.data() && enabled_.load(std::memory_order_acquire), frameStart);
    if (engine_ && aecFrames_ % 100 == 1) {
      EngineStats es;
      if (engine_->GetStats(&es)) delayState_.store(es.delayMs, std::memory_order_release);
    }
  }
}

void AecPipeline::DeliverFrame(const float* clean, bool cleanValid, int64_t frameStart) {
  Track& cap = captureTrack_;
  if (!deliver_ || cap.deliverSpans.empty() || !cap.resampler) {
    activeState_.store(false, std::memory_order_release);
    return;
  }
  // The frame holds resampler outputs [o0, o0 + kFrameSamples), which come
  // from device samples [o0 * step, (o0 + kFrameSamples) * step): the mic is
  // never trimmed, so that mapping is fixed.
  const double step = cap.resampler->Step();
  const int64_t o0 = frameStart - cap.offset;
  auto deviceIndex = [step](int64_t output) {
    const double pos = static_cast<double>(output) * step;
    return pos <= 0 ? uint64_t{0} : static_cast<uint64_t>(std::floor(pos + 1e-9));
  };
  const uint64_t frameBegin = deviceIndex(o0);
  const uint64_t frameEnd = deviceIndex(o0 + static_cast<int64_t>(kFrameSamples));
  bool deliveredClean = false;
  while (!cap.deliverSpans.empty()) {
    Track::Span& span = cap.deliverSpans.front();
    uint64_t a = std::max(cap.deliveredTo, span.begin);
    if (a >= frameEnd) break;
    const uint64_t b = std::min(span.end, frameEnd);
    if (cleanValid && a < frameBegin) {
      // Mic audio no frame covered (dropped from a backlog while the worker
      // was stalled, or cut by a resync): never lost, it goes out as captured.
      const uint64_t r = std::min(b, frameBegin);
      const size_t frameBytes = static_cast<size_t>(span.channels) * TypeBytes(span.type);
      deliver_(span.bytes.data() + (a - span.begin) * frameBytes, (r - a) * frameBytes, span.rate, span.channels,
               TypeBytes(span.type) * 8, span.type == SampleType::kFloat32);
      uncoveredSamples_ += r - a;
      a = r;
    }
    if (b > a) {
      if (cleanValid) {
        auto outIndex = [&](uint64_t input) {
          const int64_t o = static_cast<int64_t>(std::ceil(static_cast<double>(input) / step - 1e-9)) - o0;
          return std::clamp<int64_t>(o, 0, static_cast<int64_t>(kFrameSamples));
        };
        const int64_t oa = outIndex(a), ob = outIndex(b);
        if (ob > oa) {
          deliver_(reinterpret_cast<const uint8_t*>(clean + oa), static_cast<size_t>(ob - oa) * sizeof(float),
                   kProcessRate, 1, 32, true);
        }
        deliveredClean = true;
      } else {
        const size_t frameBytes = static_cast<size_t>(span.channels) * TypeBytes(span.type);
        deliver_(span.bytes.data() + (a - span.begin) * frameBytes, (b - a) * frameBytes, span.rate, span.channels,
                 TypeBytes(span.type) * 8, span.type == SampleType::kFloat32);
      }
    }
    cap.deliveredTo = std::max(cap.deliveredTo, b);
    if (cap.deliveredTo < span.end) break;
    cap.deliverSpans.pop_front();
    route_.fetch_sub(1, std::memory_order_acq_rel);
  }
  cap.deliveredTo = std::max(cap.deliveredTo, frameEnd);
  activeState_.store(deliveredClean, std::memory_order_release);
}

void AecPipeline::FlushDelivery() {
  Track& cap = captureTrack_;
  while (!cap.deliverSpans.empty()) {
    Track::Span& span = cap.deliverSpans.front();
    const uint64_t a = std::max(cap.deliveredTo, span.begin);
    if (deliver_ && span.end > a) {
      const size_t frameBytes = static_cast<size_t>(span.channels) * TypeBytes(span.type);
      deliver_(span.bytes.data() + (a - span.begin) * frameBytes, (span.end - a) * frameBytes, span.rate,
               span.channels, TypeBytes(span.type) * 8, span.type == SampleType::kFloat32);
    }
    cap.deliveredTo = std::max(cap.deliveredTo, span.end);
    cap.deliverSpans.pop_front();
    route_.fetch_sub(1, std::memory_order_acq_rel);
  }
  activeState_.store(false, std::memory_order_release);
}

void AecPipeline::UpdateRouting() {
  // Switched off while packets still come through here: hand back everything
  // held (as original bytes), then return delivery to the capture thread, but
  // only at a moment when nothing is in flight, so no packet is reordered.
  if (enabled_.load(std::memory_order_acquire) || !(route_.load(std::memory_order_acquire) & kRouteBit)) {
    return;
  }
  FlushDelivery();
  uint64_t idle = kRouteBit;
  if (route_.compare_exchange_strong(idle, 0, std::memory_order_acq_rel)) {
    emit_("aec_delivery_returned_to_capture");
  }
}

// Mic audio that could not take its normal path: reported as it happens (at
// most once a second, a stall drops in one burst), not only in aec_stats.
void AecPipeline::ReportDeliveryTrouble() {
  const uint64_t fallbacks = routeFallbacks_.load(std::memory_order_relaxed);
  if (lateFramesDropped_ == lateFramesReported_ && fallbacks == routeFallbacksReported_) {
    return;
  }
  const uint64_t now = now_();
  if (lastTroubleHost_ != 0 && now - lastTroubleHost_ < kHostTicksPerSecond) {
    return;
  }
  lastTroubleHost_ = now;
  if (lateFramesDropped_ != lateFramesReported_) {
    // The worker fell behind: those frames skipped cancellation, and the mic
    // audio in them went out as captured.
    emit_(Fmt("aec_backlog_dropped frames=%llu total=%llu",
              static_cast<unsigned long long>(lateFramesDropped_ - lateFramesReported_),
              static_cast<unsigned long long>(lateFramesDropped_)));
    lateFramesReported_ = lateFramesDropped_;
  }
  if (fallbacks != routeFallbacksReported_) {
    // Queues full: the capture thread delivered these packets itself.
    emit_(Fmt("aec_queue_full packets=%llu total=%llu",
              static_cast<unsigned long long>(fallbacks - routeFallbacksReported_),
              static_cast<unsigned long long>(fallbacks)));
    routeFallbacksReported_ = fallbacks;
  }
}

void AecPipeline::EmitStats(const char* kind) {
  EngineStats es;
  const bool haveEngineStats = engine_ && engine_->GetStats(&es);
  const char* mode = engine_ && renderTrack_.active  ? "aec"
                     : engineFailed_                 ? "passthrough_failed"
                     : engineUnavailable_            ? "passthrough_no_engine"
                     : !renderTrack_.active          ? "passthrough_no_reference"
                                                     : "passthrough";
  // aec_stats reports levels for its own period, aec_stopped for the session.
  sessionLevels_.mic += periodLevels_.mic;
  sessionLevels_.clean += periodLevels_.clean;
  sessionLevels_.reference += periodLevels_.reference;
  sessionLevels_.samples += periodLevels_.samples;
  const Levels& lv = std::strcmp(kind, "aec_stopped") == 0 ? sessionLevels_ : periodLevels_;
  auto dbfs = [&](double sumSquares) {
    return lv.samples ? 10.0 * std::log10(sumSquares / static_cast<double>(lv.samples) + 1e-12) : -120.0;
  };
  const double micDb = dbfs(lv.mic), cleanDb = dbfs(lv.clean), refDb = dbfs(lv.reference);
  const std::string levels = Fmt(" mic_dbfs=%.1f clean_dbfs=%.1f ref_dbfs=%.1f reduction_db=%.1f", micDb, cleanDb,
                                 refDb, micDb - cleanDb);
  // Session totals: mic latency added by the pipeline, and processing cost.
  const std::string cost = Fmt(
      " latency_ms_avg=%.1f latency_ms_max=%.1f ref_late_frames=%llu process_us_avg=%.0f process_us_max=%llu"
      " enabled=%d delivering_clean=%d route_fallbacks=%llu uncovered_ms=%llu",
      latencyFrames_ ? static_cast<double>(latencySumTicks_) / latencyFrames_ / 10000.0 : 0.0,
      static_cast<double>(latencyMaxTicks_) / 10000.0, static_cast<unsigned long long>(referenceLateFrames_),
      captureFrames_ ? static_cast<double>(processSumUs_) / captureFrames_ : 0.0,
      static_cast<unsigned long long>(processMaxUs_), enabled_.load() ? 1 : 0, activeState_.load() ? 1 : 0,
      static_cast<unsigned long long>(routeFallbacks_.load()),
      static_cast<unsigned long long>(uncoveredSamples_ * 1000 / kProcessRate));
  periodLevels_ = Levels();

  emit_(Fmt("%s mode=%s capture_frames=%llu aec_frames=%llu passthrough_frames=%llu ref_missing_ms=%llu "
            "engine_errors=%llu resets=%llu mic_resyncs=%llu system_resyncs=%llu mic_dropped=%llu "
            "system_dropped=%llu late_frames_dropped=%llu delay_ms=%d erle_db=%.1f",
            kind, mode, static_cast<unsigned long long>(captureFrames_), static_cast<unsigned long long>(aecFrames_),
            static_cast<unsigned long long>(passthroughFrames_),
            static_cast<unsigned long long>(referenceMissingSamples_ * 1000 / kProcessRate),
            static_cast<unsigned long long>(engineErrors_), static_cast<unsigned long long>(resets_),
            static_cast<unsigned long long>(captureTrack_.resyncs),
            static_cast<unsigned long long>(renderTrack_.resyncs),
            static_cast<unsigned long long>(capture_.dropped.load(std::memory_order_relaxed)),
            static_cast<unsigned long long>(render_.dropped.load(std::memory_order_relaxed)),
            static_cast<unsigned long long>(lateFramesDropped_), haveEngineStats ? es.delayMs : -1,
            haveEngineStats ? es.erleDb : -1.0) +
        levels + cost);
}

}  // namespace sayso::aec
