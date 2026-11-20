#include "echo_pipeline.h"

#include <time.h>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

#include "chunk_ring.h"
#include "echo_engine.h"
#include "reference_aligner.h"
#include "stream_resampler.h"

namespace sayso::echo {
namespace {

using Clock = std::chrono::steady_clock;

constexpr size_t kFrame = EchoPipeline::kFrameSamples;

constexpr double kGapNs = 20e6;
constexpr double kFillMaxNs = 1e9;
constexpr double kOffsetTauNs = 2e9;
constexpr int kOutlierChunks = 3;
constexpr double kStepMinNs = 2.5e6;
constexpr double kStepJitterFactor = 8;
constexpr int kStepChunks = 2;
constexpr double kStepWarmupSec = 2.0;
constexpr double kDeviceSettleNs = 300e6;
constexpr double kRefAliveNs = 500e6;
constexpr int64_t kRefHistory = 1024;
constexpr auto kIdleWait = std::chrono::milliseconds(5);
constexpr double kSlipRateAlpha = 0.002;
constexpr int kIngestBatch = 16;

struct Stream {
  StreamResampler rs;
  int rate = 0;
  std::vector<float> fifo;
  size_t head = 0;
  int64_t base = 0;
  int64_t inFrames = 0;
  int64_t shift = 0;
  double offsetNs = 0;
  bool haveOffset = false;
  int outliers = 0;
  int stepRun = 0;
  double devMeanNs = 0;
  double jitterNs = 0;
  double noiseNs = 0;
  Clock::time_point lastArrival{};

  size_t available() const { return fifo.size() - head; }
  int64_t total() const { return base + static_cast<int64_t>(available()); }
  void Restart() {
    fifo.clear();
    head = 0;
    base = 0;
    inFrames = 0;
    shift = 0;
    offsetNs = 0;
    haveOffset = false;
    outliers = 0;
    jitterNs = 0;
    noiseNs = 0;
    devMeanNs = 0;
    stepRun = 0;
  }
  void Compact() {
    if (head > 16384) {
      fifo.erase(fifo.begin(), fifo.begin() + static_cast<long>(head));
      head = 0;
    }
  }
};

long long ThreadCpuNs() {
  timespec ts;
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
  return static_cast<long long>(ts.tv_sec) * 1000000000LL + ts.tv_nsec;
}

}

struct EchoPipeline::Impl {
  Impl(FrameSink s, Config c)
      : sink(std::move(s)), cfg(c), micRing(c.micRingSlots), refRing(c.refRingSlots), aligner(c.aligner), policy(c.policy) {
    setting.store(c.setting);
  }

  FrameSink sink;
  Config cfg;
  ChunkRing micRing, refRing;
  std::atomic_flag micBusy = ATOMIC_FLAG_INIT, refBusy = ATOMIC_FLAG_INIT;

  std::atomic<bool> running{false};
  std::atomic<Setting> setting{Setting::kAuto};
  std::atomic<OutputKind> output{OutputKind::kUnknown};
  std::thread worker;
  std::mutex wakeMu;
  std::condition_variable cv;

  EchoEngine engine;

  Stream mic, ref;
  ReferenceAligner aligner;
  EchoPolicy policy;
  const char* resyncCause = nullptr;
  uint64_t seenDeviceSeq = 0;
  bool devicePending = false;
  double devicePendingSinceNs = 0;
  bool haveDecision = false;
  bool curProcess = false;
  Reason curReason = Reason::kDisabled;
  bool waiting = false;
  bool skipWait = false;
  Clock::time_point waitStart;
  uint64_t framesSinceStats = 0;
  long long apmNs = 0;
  double slipRate = 0;

  std::atomic<uint64_t> framesOut{0}, framesNoReference{0}, refWaitTimeouts{0}, resyncs{0}, gapsFilled{0}, slips{0};
  std::atomic<int64_t> netSlipped{0};
  std::atomic<double> lastRefError{0};
  std::atomic<uint64_t> micDropped{0}, refDropped{0}, concurrentDropped{0}, stepsSnapped{0};
  std::atomic<bool> engineAvailable{false};
  std::atomic<bool> pubProcessing{false}, pubRefActive{false};
  std::atomic<Reason> pubReason{Reason::kDisabled};
  std::atomic<uint64_t> transitions{0}, deviceSeq{0}, engineResyncs{0};
  std::atomic<double> pubRefBufferedMs{0};
  mutable std::mutex statsMu;
  double offsetMs = 0, erleDb = 0, apmDelayMs = 0, apmCpuPct = 0, driftPpm = 0;

  template <typename T>
  void Push(ChunkRing& ring, std::atomic_flag& busy, std::atomic<uint64_t>& dropped, const T* data, size_t frames,
            int channels, int rate, uint64_t ns) {
    if (!running.load(std::memory_order_acquire)) return;
    if (busy.test_and_set(std::memory_order_acquire)) {
      concurrentDropped.fetch_add(1, std::memory_order_relaxed);
      return;
    }
    const bool ok = ring.Push(data, frames, channels, rate, ns);
    busy.clear(std::memory_order_release);
    if (!ok) {
      dropped.fetch_add(1, std::memory_order_relaxed);
      return;
    }
    cv.notify_one();
  }

  void Ingest(Stream& s, const ChunkRing::Slot& c, bool isMic) {
    bool restarted = false;
    const int rate = c.rate;
    if (rate != s.rate) {
      const bool first = s.rate == 0;
      s.rate = rate;
      s.rs.Configure(rate, kSampleRate());
      s.Restart();
      if (!first) {
        resyncs.fetch_add(1, std::memory_order_relaxed);
        resyncCause = isMic ? "mic_format" : "reference_format";
      }
      restarted = true;
    }
    const size_t frames = c.frames;
    const size_t ch = c.channels;
    const double step = static_cast<double>(rate) / kSampleRate();
    const double chunkNs = static_cast<double>(frames) / rate * 1e9;

    const double predicted = static_cast<double>(s.inFrames) / step + static_cast<double>(s.shift);
    const double measured = static_cast<double>(c.ns) - predicted * kSampleNs;
    if (!s.haveOffset) {
      s.offsetNs = measured;
      s.haveOffset = true;
    } else {
      const double dev = measured - s.offsetNs;
      if (dev > kGapNs) {
        const int64_t gap = std::llround(dev / kSampleNs);
        if (dev <= kFillMaxNs) {
          s.fifo.insert(s.fifo.end(), static_cast<size_t>(gap), 0.f);
        } else {
          s.base = s.total() + gap;
          s.fifo.clear();
          s.head = 0;
          restarted = true;
          if (!resyncCause) resyncCause = "gap";
        }
        s.shift += gap;
        s.outliers = 0;
        gapsFilled.fetch_add(1, std::memory_order_relaxed);
      } else if (dev < -kGapNs) {
        if (++s.outliers >= kOutlierChunks) {
          s.Restart();
          s.offsetNs = static_cast<double>(c.ns);
          s.haveOffset = true;
          resyncs.fetch_add(1, std::memory_order_relaxed);
          restarted = true;
          if (!resyncCause) resyncCause = isMic ? "mic_format" : "reference_format";
        }
      } else {
        s.outliers = 0;
        const double alpha = 1.0 - std::exp(-chunkNs / kOffsetTauNs);
        const double detrended = dev - s.devMeanNs;
        const double stepThreshold = std::max(kStepMinNs, kStepJitterFactor * s.jitterNs);
        if (std::fabs(detrended) > stepThreshold) {
          const int dir = detrended > 0 ? 1 : -1;
          s.stepRun = (s.stepRun != 0 && (s.stepRun > 0) == (dir > 0)) ? s.stepRun + dir : dir;
          s.jitterNs += alpha * (std::min(std::fabs(detrended), stepThreshold) - s.jitterNs);
          const bool warm = static_cast<double>(s.inFrames) / rate >= kStepWarmupSec;
          if (warm && std::abs(s.stepRun) >= kStepChunks) {
            s.offsetNs = measured;
            s.devMeanNs = 0;
            s.stepRun = 0;
            stepsSnapped.fetch_add(1, std::memory_order_relaxed);
            restarted = true;
          }
        } else {
          s.stepRun = 0;
          s.devMeanNs += alpha * (dev - s.devMeanNs);
          s.jitterNs += alpha * (std::fabs(dev - s.devMeanNs) - s.jitterNs);
          s.noiseNs = s.jitterNs * std::sqrt(alpha / (2.0 - alpha));
          s.offsetNs += alpha * dev;
        }
      }
    }

    mono.resize(frames);
    if (ch == 1) {
      std::copy(c.data, c.data + frames, mono.begin());
    } else {
      const float inv = 1.f / static_cast<float>(ch);
      for (size_t i = 0; i < frames; i++) {
        float acc = 0;
        for (size_t k = 0; k < ch; k++) acc += c.data[i * ch + k];
        mono[i] = acc * inv;
      }
    }
    s.rs.Process(mono.data(), frames, s.fifo);
    s.inFrames += static_cast<int64_t>(frames);
    s.lastArrival = Clock::now();
    if (restarted) aligner.Reset();
  }
  std::vector<float> mono;

  static constexpr int kSampleRate() { return EchoPipeline::kSampleRate; }

  void DrainRing(ChunkRing& ring, Stream& s, bool isMic) {
    for (int n = 0; n < kIngestBatch; n++) {
      const ChunkRing::Slot* slot = ring.Front();
      if (!slot) return;
      Ingest(s, *slot, isMic);
      ring.Pop();
    }
  }

  bool ReferenceActive(double micFrameNs) const {
    if (!ref.haveOffset || ref.rate == 0) return false;
    const double refEndNs = ref.offsetNs + static_cast<double>(ref.total()) * kSampleNs;
    return micFrameNs - refEndNs < kRefAliveNs;
  }

  void ApplyDecision(const PolicyDecision& d) {
    if (haveDecision && d.process == curProcess && d.reason == curReason) return;
    const bool entering = d.process && !(haveDecision && curProcess);
    if (entering) {
      if (haveDecision) engine.Reset();
      aligner.Reset();
      waiting = false;
      skipWait = false;
    }
    haveDecision = true;
    curProcess = d.process;
    curReason = d.reason;
    transitions.fetch_add(1, std::memory_order_relaxed);
    pubProcessing.store(d.process, std::memory_order_relaxed);
    pubReason.store(d.reason, std::memory_order_relaxed);
    if (cfg.onEvent) {
      EchoEvent e;
      e.processing = d.process;
      e.reason = d.reason;
      e.setting = setting.load(std::memory_order_relaxed);
      e.output = output.load(std::memory_order_relaxed);
      cfg.onEvent(e);
    }
  }

  void ApplyResync(const char* cause) {
    if (engine.running()) engine.Reset();
    aligner.Reset();
    waiting = false;
    skipWait = false;
    engineResyncs.fetch_add(1, std::memory_order_relaxed);
    if (cfg.onEvent) {
      EchoEvent e;
      e.kind = EchoEvent::Kind::kResync;
      e.cause = cause;
      e.processing = curProcess;
      e.reason = curReason;
      e.setting = setting.load(std::memory_order_relaxed);
      e.output = output.load(std::memory_order_relaxed);
      cfg.onEvent(e);
    }
  }

  void ProcessFrames() {
    while (mic.available() >= kFrame) {
      const double micFrameNs = static_cast<double>(mic.base) * kSampleNs + mic.offsetNs;
      const bool refAlive = ReferenceActive(micFrameNs);
      PolicyInputs in;
      in.setting = setting.load(std::memory_order_relaxed);
      in.output = output.load(std::memory_order_relaxed);
      in.engineAvailable = engine.running();
      in.referenceActive = refAlive;
      in.nowNs = static_cast<uint64_t>(std::max(0.0, micFrameNs));
      const PolicyDecision decision = policy.Decide(in);
      ApplyDecision(decision);

      if (resyncCause) {
        const char* cause = resyncCause;
        resyncCause = nullptr;
        ApplyResync(cause);
      }
      const uint64_t seq = deviceSeq.load(std::memory_order_relaxed);
      if (seq != seenDeviceSeq) {
        seenDeviceSeq = seq;
        devicePending = true;
        devicePendingSinceNs = micFrameNs;
      }
      if (devicePending && micFrameNs - devicePendingSinceNs >= kDeviceSettleNs) {
        devicePending = false;
        ApplyResync("device_change");
      }
      pubRefActive.store(refAlive, std::memory_order_relaxed);
      const bool process = decision.process;

      float refFrame[kFrame] = {0};
      bool refMissing = false;

      if (process) {
        if (refAlive) {
          const double noiseSamples = std::hypot(mic.noiseNs, ref.noiseNs) / kSampleNs;
          const ReferenceAligner::Plan plan = aligner.Next(micFrameNs, ref.offsetNs, kFrame, noiseSamples);
          if (ref.total() < plan.start + static_cast<int64_t>(kFrame)) {
            if (!skipWait) {
              if (!waiting) {
                waiting = true;
                waitStart = Clock::now();
              }
              if (Clock::now() - waitStart < std::chrono::milliseconds(cfg.maxReferenceWaitMs)) return;
              skipWait = true;
              refWaitTimeouts.fetch_add(1, std::memory_order_relaxed);
            }
          } else {
            skipWait = false;
          }
          waiting = false;
          for (size_t i = 0; i < kFrame; i++) {
            const int64_t idx = plan.start + static_cast<int64_t>(i);
            if (idx >= ref.base && idx < ref.total()) {
              refFrame[i] = ref.fifo[ref.head + static_cast<size_t>(idx - ref.base)];
            } else {
              refMissing = true;
            }
          }
          if (plan.resynced) resyncs.fetch_add(1, std::memory_order_relaxed);
          if (plan.slip != 0) {
            slips.fetch_add(1, std::memory_order_relaxed);
            netSlipped.fetch_add(plan.slip, std::memory_order_relaxed);
          }
          slipRate += kSlipRateAlpha * (static_cast<double>(plan.slip) / kFrame - slipRate);
          lastRefError.store(plan.errorSamples, std::memory_order_relaxed);
          aligner.Commit(plan);
          const int64_t keepFrom = aligner.position() - kRefHistory;
          if (keepFrom > ref.base) {
            const size_t drop = static_cast<size_t>(std::min<int64_t>(keepFrom - ref.base, static_cast<int64_t>(ref.available())));
            ref.head += drop;
            ref.base += static_cast<int64_t>(drop);
            ref.Compact();
          }
        } else {
          waiting = false;
          aligner.Reset();
          refMissing = true;
        }
      } else {
        waiting = false;
        if (ref.haveOffset) {
          const int64_t keepFrom = static_cast<int64_t>((micFrameNs - ref.offsetNs) / kSampleNs) - kRefHistory;
          if (keepFrom > ref.base) {
            const size_t drop = static_cast<size_t>(std::min<int64_t>(keepFrom - ref.base, static_cast<int64_t>(ref.available())));
            ref.head += drop;
            ref.base += static_cast<int64_t>(drop);
            ref.Compact();
          }
        }
      }

      int16_t micS16[kFrame], outS16[kFrame];
      for (size_t i = 0; i < kFrame; i++) micS16[i] = FloatToS16(mic.fifo[mic.head + i]);

      if (process) {
        int16_t refS16[kFrame];
        for (size_t i = 0; i < kFrame; i++) refS16[i] = FloatToS16(refFrame[i]);
        const long long t0 = ThreadCpuNs();
        engine.ProcessReference(refS16);
        engine.ProcessCapture(micS16, outS16);
        apmNs += ThreadCpuNs() - t0;
        if (refMissing) framesNoReference.fetch_add(1, std::memory_order_relaxed);
      } else {
        std::copy(micS16, micS16 + kFrame, outS16);
      }

      double stampNs = micFrameNs - (process ? static_cast<double>(kCaptureLatencySamples) * kSampleNs : 0.0);
      if (stampNs < 0) stampNs = 0;

      mic.head += kFrame;
      mic.base += static_cast<int64_t>(kFrame);
      mic.Compact();
      framesOut.fetch_add(1, std::memory_order_relaxed);
      sink(outS16, kFrame, static_cast<uint64_t>(stampNs));

      if (++framesSinceStats >= 100) {
        framesSinceStats = 0;
        PublishStats();
      }
    }
  }

  void PublishStats() {
    const EngineStats es = engine.PollStats();
    const uint64_t frames = framesOut.load(std::memory_order_relaxed);
    std::lock_guard<std::mutex> lk(statsMu);
    pubRefBufferedMs.store(static_cast<double>(ref.available()) / 16.0, std::memory_order_relaxed);
    offsetMs = (mic.haveOffset && ref.haveOffset) ? (mic.offsetNs - ref.offsetNs) / 1e6 : 0;
    erleDb = es.hasErle ? es.erleDb : 0;
    apmDelayMs = es.hasDelay ? es.delayMs : 0;
    driftPpm = slipRate * 1e6;
    apmCpuPct = frames > 0 ? static_cast<double>(apmNs) / (static_cast<double>(frames) * 10e6) * 100.0 : 0;
  }

  void Run() {
    while (running.load(std::memory_order_acquire)) {
      if (micRing.size() == 0 && refRing.size() == 0) {
        std::unique_lock<std::mutex> lk(wakeMu);
        cv.wait_for(lk, kIdleWait);
      }
      if (!running.load(std::memory_order_acquire)) break;
      DrainRing(refRing, ref, false);
      DrainRing(micRing, mic, true);
      ProcessFrames();
    }
  }
};

EchoPipeline::EchoPipeline(FrameSink sink) : EchoPipeline(std::move(sink), Config()) {}
EchoPipeline::EchoPipeline(FrameSink sink, Config config) : impl_(std::make_unique<Impl>(std::move(sink), config)) {}
EchoPipeline::~EchoPipeline() { Stop(); }

bool EchoPipeline::Start() {
  Impl& d = *impl_;
  if (d.running.load()) return true;
  d.engineAvailable.store(d.engine.Start());
  d.mic = Stream();
  d.ref = Stream();
  d.aligner.Reset();
  d.policy.Reset();
  d.resyncCause = nullptr;
  d.seenDeviceSeq = d.deviceSeq.load();
  d.devicePending = false;
  d.engineResyncs = 0;
  d.haveDecision = false;
  d.curProcess = false;
  d.curReason = Reason::kDisabled;
  d.transitions = 0;
  d.pubProcessing = false;
  d.pubRefActive = false;
  d.pubReason = Reason::kDisabled;
  d.pubRefBufferedMs = 0;
  d.waiting = false;
  d.skipWait = false;
  d.framesSinceStats = 0;
  d.apmNs = 0;
  d.slipRate = 0;
  d.framesOut = d.framesNoReference = d.refWaitTimeouts = d.resyncs = d.gapsFilled = d.slips = 0;
  d.netSlipped = 0;
  d.lastRefError = 0;
  d.micDropped = d.refDropped = d.concurrentDropped = d.stepsSnapped = 0;
  d.micRing.Clear();
  d.refRing.Clear();
  {
    std::lock_guard<std::mutex> lk(d.statsMu);
    d.offsetMs = d.erleDb = d.apmDelayMs = d.apmCpuPct = d.driftPpm = 0;
  }
  d.running.store(true, std::memory_order_release);
  d.worker = std::thread([this] { impl_->Run(); });
  return true;
}

void EchoPipeline::Stop() {
  Impl& d = *impl_;
  if (!d.running.exchange(false) && !d.worker.joinable()) return;
  d.cv.notify_all();
  if (d.worker.joinable()) d.worker.join();
  d.engine.Stop();
  d.engineAvailable.store(false);
  d.micRing.Clear();
  d.refRing.Clear();
}

void EchoPipeline::PushMic(const float* p, size_t frames, int channels, int rate, uint64_t ns) {
  impl_->Push(impl_->micRing, impl_->micBusy, impl_->micDropped, p, frames, channels, rate, ns);
}
void EchoPipeline::PushMic(const int16_t* p, size_t frames, int channels, int rate, uint64_t ns) {
  impl_->Push(impl_->micRing, impl_->micBusy, impl_->micDropped, p, frames, channels, rate, ns);
}
void EchoPipeline::PushReference(const float* p, size_t frames, int channels, int rate, uint64_t ns) {
  impl_->Push(impl_->refRing, impl_->refBusy, impl_->refDropped, p, frames, channels, rate, ns);
}
void EchoPipeline::PushReference(const int16_t* p, size_t frames, int channels, int rate, uint64_t ns) {
  impl_->Push(impl_->refRing, impl_->refBusy, impl_->refDropped, p, frames, channels, rate, ns);
}

void EchoPipeline::SetSetting(Setting setting) { impl_->setting.store(setting); }
void EchoPipeline::SetOutputKind(OutputKind kind) { impl_->output.store(kind); }
void EchoPipeline::NotifyDeviceChange() { impl_->deviceSeq.fetch_add(1, std::memory_order_relaxed); }

PipelineStats EchoPipeline::GetStats() const {
  const Impl& d = *impl_;
  PipelineStats s;
  s.running = d.running.load();
  s.processing = d.pubProcessing.load();
  s.reason = d.pubReason.load();
  s.setting = d.setting.load();
  s.output = d.output.load();
  s.referenceActive = d.pubRefActive.load();
  s.transitions = d.transitions.load();
  s.deviceChanges = d.deviceSeq.load();
  s.engineResyncs = d.engineResyncs.load();
  s.refBufferedMs = d.pubRefBufferedMs.load();
  s.engineAvailable = d.engineAvailable.load();
  s.framesOut = d.framesOut.load();
  s.framesNoReference = d.framesNoReference.load();
  s.refWaitTimeouts = d.refWaitTimeouts.load();
  s.resyncs = d.resyncs.load();
  s.gapsFilled = d.gapsFilled.load();
  s.micChunksDropped = d.micDropped.load();
  s.refChunksDropped = d.refDropped.load();
  s.concurrentPushDropped = d.concurrentDropped.load();
  s.stepsSnapped = d.stepsSnapped.load();
  s.slips = d.slips.load();
  s.netSlippedSamples = d.netSlipped.load();
  s.refErrorSamples = d.lastRefError.load();
  std::lock_guard<std::mutex> lk(d.statsMu);
  s.offsetMs = d.offsetMs;
  s.erleDb = d.erleDb;
  s.apmDelayMs = d.apmDelayMs;
  s.apmCpuPct = d.apmCpuPct;
  s.driftPpm = d.driftPpm;
  return s;
}

}
