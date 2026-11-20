#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace sayso::echo {

constexpr double kSampleNs = 1e9 / 16000.0;

class ReferenceAligner {
 public:
  struct Config {
    bool driftCorrection = true;
    double slipThresholdSamples = 4;
    double maxThresholdSamples = 64;
    double noiseFactor = 4;
    double hardResyncSamples = 800;
    int maxSlipPerFrame = 8;
  };

  struct Plan {
    int64_t start = 0;
    int64_t advance = 0;
    int slip = 0;
    bool resynced = false;
    double errorSamples = 0;
  };

  ReferenceAligner() = default;
  explicit ReferenceAligner(const Config& c) : cfg_(c) {}

  void Reset() { valid_ = false; pos_ = 0; }
  bool valid() const { return valid_; }
  const Config& config() const { return cfg_; }

  Plan Next(double micFrameNs, double refOffsetNs, size_t frameSamples, double noiseSamples = 0) const {
    Plan p;
    p.advance = static_cast<int64_t>(frameSamples);
    const double desired = (micFrameNs - refOffsetNs) / kSampleNs;
    if (!valid_) {
      p.start = std::llround(desired);
      return p;
    }
    const double error = desired - static_cast<double>(pos_);
    p.errorSamples = error;
    if (std::fabs(error) > cfg_.hardResyncSamples) {
      p.start = std::llround(desired);
      p.resynced = true;
      return p;
    }
    p.start = pos_;
    if (cfg_.driftCorrection) {
      const double threshold = std::clamp(cfg_.noiseFactor * noiseSamples, cfg_.slipThresholdSamples, cfg_.maxThresholdSamples);
      if (std::fabs(error) >= threshold) {
        const int step = std::min(cfg_.maxSlipPerFrame, 1 + static_cast<int>(std::fabs(error) / 64.0));
        p.slip = error > 0 ? step : -step;
        p.advance += p.slip;
      }
    }
    return p;
  }

  void Commit(const Plan& p) { pos_ = p.start + p.advance; valid_ = true; }

  int64_t position() const { return pos_; }

 private:
  Config cfg_;
  bool valid_ = false;
  int64_t pos_ = 0;
};

}
