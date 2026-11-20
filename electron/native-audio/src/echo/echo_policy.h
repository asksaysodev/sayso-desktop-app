#pragma once

#include <cstdint>

namespace sayso::echo {

enum class Setting { kOff, kAuto, kOn };

enum class OutputKind { kUnknown, kSpeakers, kHeadphones };

enum class Reason {
  kActive,
  kDisabled,
  kHeadphones,
  kEngineUnavailable,
  kNoReference,
};

inline const char* ToString(Setting s) { return s == Setting::kOff ? "off" : (s == Setting::kOn ? "on" : "auto"); }
inline const char* ToString(OutputKind k) { return k == OutputKind::kSpeakers ? "speakers" : (k == OutputKind::kHeadphones ? "headphones" : "unknown"); }
inline const char* ToString(Reason r) {
  switch (r) {
    case Reason::kActive: return "active";
    case Reason::kDisabled: return "disabled";
    case Reason::kHeadphones: return "headphones";
    case Reason::kEngineUnavailable: return "engine_unavailable";
    case Reason::kNoReference: return "no_reference";
  }
  return "unknown";
}

struct PolicyInputs {
  Setting setting = Setting::kAuto;
  OutputKind output = OutputKind::kUnknown;
  bool engineAvailable = false;
  bool referenceActive = false;
  uint64_t nowNs = 0;
};

struct PolicyDecision {
  bool process = false;
  Reason reason = Reason::kDisabled;
};

class EchoPolicy {
 public:
  struct Config {
    uint64_t noReferenceAfterNs = 1000000000ull;
    uint64_t referenceBackAfterNs = 500000000ull;
  };

  EchoPolicy() = default;
  explicit EchoPolicy(const Config& c) : cfg_(c) {}

  void Reset() { started_ = false; }

  PolicyDecision Decide(const PolicyInputs& in) {
    if (in.setting == Setting::kOff) return Pin({false, Reason::kDisabled}, in);
    if (!in.engineAvailable) return Pin({false, Reason::kEngineUnavailable}, in);
    if (in.setting == Setting::kAuto && in.output == OutputKind::kHeadphones) return Pin({false, Reason::kHeadphones}, in);

    if (!started_) {
      started_ = true;
      refBad_ = !in.referenceActive;
      sinceNs_ = in.nowNs;
      noRef_ = false;
    }
    if (in.referenceActive != !refBad_) {
      refBad_ = !in.referenceActive;
      sinceNs_ = in.nowNs;
    }
    const uint64_t run = in.nowNs >= sinceNs_ ? in.nowNs - sinceNs_ : 0;
    if (!noRef_ && refBad_ && run >= cfg_.noReferenceAfterNs) noRef_ = true;
    if (noRef_ && !refBad_ && run >= cfg_.referenceBackAfterNs) noRef_ = false;
    return noRef_ ? PolicyDecision{false, Reason::kNoReference} : PolicyDecision{true, Reason::kActive};
  }

 private:
  PolicyDecision Pin(PolicyDecision d, const PolicyInputs&) {
    started_ = false;
    return d;
  }

  Config cfg_;
  bool started_ = false;
  bool refBad_ = false;
  bool noRef_ = false;
  uint64_t sinceNs_ = 0;
};

}
