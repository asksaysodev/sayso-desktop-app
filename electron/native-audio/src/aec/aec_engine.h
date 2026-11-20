// The echo-cancellation engine behind the pipeline. One implementation today
// (WebRTC AEC3, linked statically); the pipeline only sees this interface, so
// the engine and how it is packaged can change without touching capture code.

#ifndef SAYSO_AEC_AEC_ENGINE_H_
#define SAYSO_AEC_AEC_ENGINE_H_

#include <memory>
#include <string>

namespace sayso::aec {

struct EngineStats {
  int delayMs = -1;   // -1 = not estimated yet
  double erlDb = -1;  // echo return loss
  double erleDb = -1; // echo return loss enhancement: how much echo was removed
};

class AecEngine {
 public:
  virtual ~AecEngine() = default;
  virtual const char* Name() const = 0;
  // One 10 ms mono frame each. Return 0 on success, an engine error code otherwise.
  virtual int ProcessRender(const float* frame) = 0;
  virtual int ProcessCapture(const float* in, float* out) = 0;
  virtual bool GetStats(EngineStats* stats) = 0;
};

// Returns nullptr when no engine can run, with the reason in `failReason`
// ("not_built": this addon was compiled without the WebRTC library;
// "create_failed": the library refused the configuration).
std::unique_ptr<AecEngine> CreateAecEngine(int sampleRate, std::string* failReason);

}  // namespace sayso::aec

#endif  // SAYSO_AEC_AEC_ENGINE_H_
