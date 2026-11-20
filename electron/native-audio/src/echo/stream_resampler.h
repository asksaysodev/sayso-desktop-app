#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <vector>

namespace sayso::echo {

class StreamResampler {
 public:
  void Configure(int inRate, int outRate) {
    inRate_ = inRate;
    outRate_ = outRate;
    step_ = static_cast<double>(inRate) / static_cast<double>(outRate);
    fc_ = 0.9 * std::min(1.0, static_cast<double>(outRate) / static_cast<double>(inRate));
    half_ = static_cast<long>(std::ceil(kZeroCrossings / fc_));
    const size_t n = static_cast<size_t>(2 * half_ * kPhases) + 2;
    table_.assign(n, 0.f);
    for (size_t i = 0; i < n; i++) {
      const double t = static_cast<double>(i) / kPhases - static_cast<double>(half_);
      const double x = fc_ * t;
      const double sinc = std::fabs(x) < 1e-12 ? 1.0 : std::sin(M_PI * x) / (M_PI * x);
      const double hann = std::fabs(t) >= static_cast<double>(half_) ? 0.0 : 0.5 + 0.5 * std::cos(M_PI * t / static_cast<double>(half_));
      table_[i] = static_cast<float>(fc_ * sinc * hann);
    }
    buf_.assign(static_cast<size_t>(half_), 0.f);
    pos_ = static_cast<double>(half_);
  }

  bool configured() const { return inRate_ > 0; }
  int inRate() const { return inRate_; }
  int outRate() const { return outRate_; }

  void Process(const float* in, size_t n, std::vector<float>& out) {
    if (!configured() || n == 0) return;
    buf_.insert(buf_.end(), in, in + n);
    const long size = static_cast<long>(buf_.size());
    while (static_cast<long>(std::floor(pos_)) + half_ < size) {
      const long c = static_cast<long>(std::floor(pos_));
      double acc = 0;
      for (long k = c - half_ + 1; k <= c + half_; k++) {
        const double t = static_cast<double>(k) - pos_;
        const double f = (t + static_cast<double>(half_)) * kPhases;
        const size_t i0 = static_cast<size_t>(f);
        const float frac = static_cast<float>(f - static_cast<double>(i0));
        acc += buf_[static_cast<size_t>(k)] * (table_[i0] + frac * (table_[i0 + 1] - table_[i0]));
      }
      out.push_back(static_cast<float>(acc));
      pos_ += step_;
    }
    const long keepFrom = static_cast<long>(std::floor(pos_)) - half_ + 1;
    if (keepFrom > 0) {
      buf_.erase(buf_.begin(), buf_.begin() + keepFrom);
      pos_ -= static_cast<double>(keepFrom);
    }
  }

 private:
  static constexpr double kZeroCrossings = 12.0;
  static constexpr double kPhases = 256.0;
  int inRate_ = 0;
  int outRate_ = 0;
  double step_ = 1;
  double fc_ = 1;
  long half_ = 0;
  double pos_ = 0;
  std::vector<float> buf_;
  std::vector<float> table_;
};

}
