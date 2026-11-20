// Shared echo-cancellation pipeline: platform-free C++ used by the Windows
// build today and meant for the macOS build later. Only how timestamps are
// captured differs per OS.
//
//   capture thread (mic)    --PushPacket--> lock-free ring --\
//                                                              worker thread:
//   capture thread (system) --PushPacket--> lock-free ring --/  mono 48 kHz,
//                                                               aligned by host time,
//                                                               10 ms frames -> engine
//                                                               -> clean mic -> sink
//
// Capture threads only convert to mono float and copy into a ring; they never
// wait on the worker. The worker puts both streams on one timeline from the
// host timestamp of each packet (QPC on Windows), so the reference lines up
// with the mic by real time rather than by arrival time. Gaps in the system
// audio (it may send nothing while silent) become silence at the right place.
//
// The mic is the master clock: its audio is never shifted to follow
// timestamps, which jitter. Timestamp error is smoothed, and only the
// reference is moved to stay in line with it; the mic is re-placed only when
// its stream genuinely breaks.
//
// When there is no system-audio stream, or no engine could be created, the
// mic passes through untouched and an event says why.

#ifndef SAYSO_AEC_AEC_PIPELINE_H_
#define SAYSO_AEC_AEC_PIPELINE_H_

#include <atomic>
#include <condition_variable>
#include <deque>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "aec_engine.h"
#include "audio_format.h"
#ifdef SAYSO_AEC_DIAG_RECORDING
#include "diagnostic_recorder.h"
#endif
#include "resampler.h"
#include "spsc_ring.h"

namespace sayso::aec {

enum class AecPath { kCapture = 0, kRender = 1 };

class AecPipeline {
 public:
  // The rate the engine runs at, whatever the devices deliver. The 16 kHz wire
  // format is produced later, by the existing JS converter.
  static constexpr int kProcessRate = 48000;
  static constexpr size_t kFrameSamples = kProcessRate / 100;

  using EmitFn = void (*)(const std::string& event);
  // Current host time in 100 ns units: the clock packet timestamps are on.
  using HostClockFn = uint64_t (*)();
  // Receives every processed 10 ms mic frame, on the worker thread.
  using CleanSink = std::function<void(const float* frame, size_t samples)>;
  // Delivers mic audio to the app on the worker thread, while echo
  // cancellation is switched on: either cleaned audio (48 kHz mono float) or,
  // when it cannot cancel, the device's original bytes unchanged.
  using DeliverFn = std::function<void(const uint8_t* bytes, size_t size, double sampleRate, int channels,
                                       int bitDepth, bool isFloat)>;

  // What getEchoCancellationState reports.
  struct State {
    bool enabled = false;  // switched on
    bool active = false;   // currently delivering cleaned audio
    int delayMs = -1;      // engine's echo delay estimate, -1 = unknown
  };

  AecPipeline(EmitFn emit, HostClockFn now);
  ~AecPipeline();
  AecPipeline(const AecPipeline&) = delete;
  AecPipeline& operator=(const AecPipeline&) = delete;

  // Control. Call from one thread at a time.
  void Start();  // no-op if running
  void Stop();   // joins the worker; no-op if stopped
  void SetCleanSink(CleanSink sink);  // only while stopped
  void SetDeliverySink(DeliverFn deliver);  // only while stopped

  // The echo-cancellation switch, live: takes effect from the next mic packet
  // when switched on; when switched off, mic delivery returns to the capture
  // thread once everything the pipeline holds has been handed over (no gap,
  // overlap or reordering). Off: the mic is delivered exactly as without the
  // pipeline. Any thread.
  void SetEnabled(bool on);
  bool Enabled() const { return enabled_.load(std::memory_order_acquire); }
  bool Running() const { return running_.load(std::memory_order_acquire); }
  State GetState() const;
  // Opt-in: record reference / raw mic / clean mic WAVs into `folderUtf8`
  // (developer builds only: compiled in with SAYSO_AEC_DIAG_RECORDING; other
  // builds report aec_recording_unavailable reason=not_built)
  // for at most `maxSeconds` per session. Empty folder = no recording.
  // Only while stopped; applies from the next Start().
  void SetDiagnosticRecording(const std::string& folderUtf8, double maxSeconds);

  // Producer side, from a capture thread: at most one thread per path at a
  // time. Never blocks; when the ring is full the packet is dropped and
  // counted. A new `streamId` on a path marks a new device stream (start or
  // restart), which resets that path's alignment and the engine.
  // `data` null = a packet the device flagged silent.
  // Returns true when the pipeline has taken over delivering this mic packet
  // to the app (echo cancellation switched on): the caller must then not
  // deliver it itself. Always false for system audio.
  bool PushPacket(AecPath path, uint64_t streamId, double sampleRate, const uint8_t* data, uint32_t frames,
                  int channels, SampleType type, uint64_t hostTime, bool hostTimeValid);
  void EndStream(AecPath path, uint64_t streamId);

 private:
  struct PacketMeta {
    enum Kind : uint8_t { kData, kEnd } kind = kData;
    bool hostTimeValid = false;
    bool deliver = false;      // mic only: the pipeline delivers this packet to the app
    uint8_t channels = 0;      // original device format, for bit-exact pass-through
    SampleType type = SampleType::kUnsupported;
    uint32_t frames = 0;
    uint64_t rawPos = 0;       // where this packet's original bytes start in the raw ring
    uint64_t streamId = 0;
    uint64_t hostTime = 0;
    uint64_t arrivalHost = 0;  // host clock when the capture thread handed it over
    uint64_t samplePos = 0;  // where this packet's samples start in the sample ring
    double sampleRate = 0;
  };

  // Written by one capture thread, read by the worker.
  struct Input {
    Input();
    SpscRing<float> samples;
    SpscRing<PacketMeta> meta;
    SpscRing<uint8_t> raw;    // mic only: original bytes of packets the pipeline delivers
    std::vector<float> mono;  // producer-only scratch
    std::vector<uint8_t> zeros;  // producer-only: original bytes of a packet flagged silent
    std::atomic<uint64_t> dropped{0};
  };

  // Worker-only state for one path.
  struct Track {
    const char* name = "";
    bool active = false;
    uint64_t streamId = 0;
    std::unique_ptr<Resampler> resampler;
    uint64_t inSamples = 0;   // device samples consumed this stream
    uint64_t outSamples = 0;  // 48 kHz samples produced this stream
    bool placed = false;
    bool timestampsUntrusted = false;  // timestamps not on the shared clock: place by arrival
    uint64_t trustedPackets = 0;  // packets whose timestamps matched their arrival
    double arrivalLagEma = 0;     // usual arrival minus timestamp of those packets, in host ticks
    int64_t clockShift = 0;       // added to this stream's timestamps after its clock jumped
    uint64_t lastClockJumpHost = 0;
    int jumpPackets = 0;          // consecutive packets far off the shared clock, by about the same amount
    int64_t jumpDistance = 0;     // the first of those packets' arrival minus timestamp
    int64_t offset = 0;       // timeline index = outSamples index + offset
    double errEma = 0;        // smoothed timestamp error vs offset, in samples
    double jitterEma = 0;     // mean timing wander of this stream's packets, in samples
    bool fifoStarted = false;
    int64_t fifoStart = 0;    // timeline index of fifo[0]
    std::vector<float> fifo;  // 48 kHz samples not yet consumed
    std::vector<float> resampled;
    std::vector<float> packet;
    uint64_t resyncs = 0;
    uint64_t gapSamples = 0;
    // Mic delivery (echo cancellation switched on): the packets the pipeline
    // hands to the app, in order, each with its original bytes and format.
    struct Span {
      uint64_t begin = 0, end = 0;  // device-sample range in this stream
      std::vector<uint8_t> bytes;   // original bytes, for bit-exact pass-through
      double rate = 0;
      int channels = 0;
      SampleType type = SampleType::kUnsupported;
    };
    std::deque<Span> deliverSpans;
    uint64_t deliveredTo = 0;   // device samples of this stream handled so far
    std::vector<uint8_t> packetRaw;  // the current packet's original bytes
  };

  void WorkerMain();
  void DrainInput(AecPath path);
  void HandleData(Track& track, const PacketMeta& meta);
  void BeginTrack(Track& track, const PacketMeta& meta);
  void EndTrack(Track& track, const char* why);
  void WriteFifo(Track& track, int64_t index, const float* data, size_t n);
  int64_t TimelineIndex(uint64_t hostTime) const;
  void ProcessFrames();
  void ApplyPendingReset();
  void RequestReset(const std::string& reason);
  void EmitStats(const char* kind);
  void DeliverFrame(const float* clean, bool cleanValid, int64_t frameStart);
  void FlushDelivery();
  void UpdateRouting();
  void ReportDeliveryTrouble();
  Input& InputFor(AecPath path) { return path == AecPath::kCapture ? capture_ : render_; }
  Track& TrackFor(AecPath path) { return path == AecPath::kCapture ? captureTrack_ : renderTrack_; }

  const EmitFn emit_;
  const HostClockFn now_;
  CleanSink sink_;
  DeliverFn deliver_;
  // Mic delivery routing, shared with the capture thread, lock-free: the top
  // bit = packets go through the pipeline, the rest = packets handed to the
  // pipeline and not yet delivered. The capture thread only takes a packet
  // while the bit is set; the worker clears it only at a count of zero.
  static constexpr uint64_t kRouteBit = 1ull << 63;
  std::atomic<uint64_t> route_{0};
  std::atomic<bool> enabled_{false};
  std::atomic<bool> activeState_{false};
  std::atomic<int> delayState_{-1};
  std::atomic<uint64_t> routeFallbacks_{0};  // taken-over packets the rings could not hold
  std::string recordFolder_;
  double recordMaxSeconds_ = 0;

  Input capture_;
  Input render_;
  std::atomic<bool> running_{false};

  std::thread worker_;
  std::mutex stopMutex_;
  std::condition_variable stopCv_;
  bool stopRequested_ = false;  // guarded by stopMutex_

  // ---- worker-only from here ----
  Track captureTrack_;
  Track renderTrack_;
  bool haveOrigin_ = false;
  uint64_t originHost_ = 0;
  // How far the mic's timestamps run from its sample count (its clock error
  // plus timestamp bias), smoothed. Applied when placing the reference.
  double captureSkew_ = 0;
  std::unique_ptr<AecEngine> engine_;
  bool engineUnavailable_ = false;  // creation failed; retried on the next reset
  bool engineFailed_ = false;       // too many processing errors; retried on the next reset
  std::string pendingReset_;
  int consecutiveErrors_ = 0;
  int referenceState_ = -1;  // -1 not reported yet, 0 missing, 1 present
  std::vector<float> captureFrame_;
  std::vector<float> renderFrame_;
  std::vector<float> cleanFrame_;
  uint64_t lastStatsHost_ = 0;

  // Counters for the current session, reported in aec_stats / aec_stopped.
  uint64_t captureFrames_ = 0;
  uint64_t aecFrames_ = 0;
  uint64_t passthroughFrames_ = 0;
  uint64_t referenceMissingSamples_ = 0;
  uint64_t engineErrors_ = 0;
  uint64_t resets_ = 0;
  uint64_t lateFramesDropped_ = 0;
  uint64_t uncoveredSamples_ = 0;  // mic samples delivered as captured because no frame covered them
  // What the last delivery-trouble event already reported.
  uint64_t lateFramesReported_ = 0;
  uint64_t routeFallbacksReported_ = 0;
  uint64_t lastTroubleHost_ = 0;
  // Latency and cost, reported in aec_stats / aec_stopped.
  uint64_t renderLastArrival_ = 0;   // host time of the newest system-audio packet
  uint64_t referenceLateFrames_ = 0; // frames processed without their (playing) reference
  uint64_t latencySumTicks_ = 0;
  uint64_t latencyMaxTicks_ = 0;
  uint64_t latencyFrames_ = 0;
  uint64_t processSumUs_ = 0;        // time spent processing frames
  uint64_t processMaxUs_ = 0;

  // Signal levels, so the log shows what cancellation did without listening.
  struct Levels {
    double mic = 0, clean = 0, reference = 0;  // sums of squares
    uint64_t samples = 0;
  };
  Levels periodLevels_;   // since the last aec_stats
  Levels sessionLevels_;  // since Start()
#ifdef SAYSO_AEC_DIAG_RECORDING
  DiagnosticRecorder recorder_;
#endif
};

}  // namespace sayso::aec

#endif  // SAYSO_AEC_AEC_PIPELINE_H_
