#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

namespace sayso::echo {

constexpr int kSampleRate = 16000;
constexpr size_t kFrameSamples = 160;

constexpr size_t kCaptureLatencySamples = 128;

struct EngineStats {
  uint64_t referenceFrames = 0;
  uint64_t captureFrames = 0;
  uint64_t passthroughFrames = 0;
  uint64_t errors = 0;
  uint64_t startFailures = 0;
  uint64_t resets = 0;
  bool hasErle = false;
  double erleDb = 0;
  bool hasDelay = false;
  double delayMs = 0;
};

class EchoEngine {
 public:
  EchoEngine();
  ~EchoEngine();
  EchoEngine(const EchoEngine&) = delete;
  EchoEngine& operator=(const EchoEngine&) = delete;

  static bool IsCompiledIn();
  static const char* Version();

  bool Start();
  void Stop();
  bool running() const;
  bool Reset();

  bool ProcessReference(const int16_t* frame);

  bool ProcessCapture(const int16_t* in, int16_t* out);

  EngineStats PollStats();

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

inline int16_t FloatToS16(float v) {
  const float s = v * 32768.f;
  return static_cast<int16_t>(s > 32767.f ? 32767.f : (s < -32768.f ? -32768.f : s));
}
inline float S16ToFloat(int16_t v) { return static_cast<float>(v) * (1.f / 32768.f); }

}
