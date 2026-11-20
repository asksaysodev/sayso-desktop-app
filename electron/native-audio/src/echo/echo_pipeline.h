#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>

#include "echo_policy.h"
#include "reference_aligner.h"

namespace sayso::echo {

struct EchoEvent {
  enum class Kind {
    kState,
    kResync,
  };
  Kind kind = Kind::kState;
  const char* cause = "";
  bool processing = false;
  Reason reason = Reason::kDisabled;
  Setting setting = Setting::kAuto;
  OutputKind output = OutputKind::kUnknown;
};

inline std::string FormatEvent(const EchoEvent& e) {
  if (e.kind == EchoEvent::Kind::kResync) return std::string("echo_resync cause=") + e.cause;
  return std::string("echo_state state=") + (e.processing ? "processing" : "bypassed") + " reason=" + ToString(e.reason) +
         " setting=" + ToString(e.setting) + " output=" + ToString(e.output);
}

struct PipelineStats {
  bool running = false;
  bool processing = false;
  Reason reason = Reason::kDisabled;
  Setting setting = Setting::kAuto;
  OutputKind output = OutputKind::kUnknown;
  bool referenceActive = false;
  uint64_t transitions = 0;
  uint64_t deviceChanges = 0;
  uint64_t engineResyncs = 0;
  bool engineAvailable = false;
  uint64_t framesOut = 0;
  uint64_t framesNoReference = 0;
  uint64_t refWaitTimeouts = 0;
  uint64_t resyncs = 0;
  uint64_t gapsFilled = 0;
  uint64_t micChunksDropped = 0;
  uint64_t refChunksDropped = 0;
  uint64_t concurrentPushDropped = 0;
  uint64_t stepsSnapped = 0;
  uint64_t slips = 0;
  int64_t netSlippedSamples = 0;
  double driftPpm = 0;
  double refErrorSamples = 0;
  double refBufferedMs = 0;
  double offsetMs = 0;
  double erleDb = 0;
  double apmDelayMs = 0;
  double apmCpuPct = 0;
};

class EchoPipeline {
 public:
  static constexpr int kSampleRate = 16000;
  static constexpr size_t kFrameSamples = 160;

  using FrameSink = std::function<void(const int16_t* samples, size_t count, uint64_t hostNs)>;

  struct Config {
    size_t micRingSlots = 64;
    size_t refRingSlots = 256;
    int maxReferenceWaitMs = 60;
    ReferenceAligner::Config aligner;
    Setting setting = Setting::kAuto;
    EchoPolicy::Config policy;
    std::function<void(const EchoEvent&)> onEvent;
  };

  explicit EchoPipeline(FrameSink sink);
  EchoPipeline(FrameSink sink, Config config);
  ~EchoPipeline();
  EchoPipeline(const EchoPipeline&) = delete;
  EchoPipeline& operator=(const EchoPipeline&) = delete;

  bool Start();
  void Stop();

  void PushMic(const float* interleaved, size_t frames, int channels, int sampleRate, uint64_t hostNs);
  void PushMic(const int16_t* interleaved, size_t frames, int channels, int sampleRate, uint64_t hostNs);
  void PushReference(const float* interleaved, size_t frames, int channels, int sampleRate, uint64_t hostNs);
  void PushReference(const int16_t* interleaved, size_t frames, int channels, int sampleRate, uint64_t hostNs);

  void SetSetting(Setting setting);
  void SetOutputKind(OutputKind kind);
  void NotifyDeviceChange();
  PipelineStats GetStats() const;

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

}
