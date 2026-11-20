#include "resampler.h"

#include <algorithm>
#include <cmath>

namespace sayso::aec {

namespace {

constexpr double kPi = 3.14159265358979323846;

// Blackman window over [-1, 1].
double Window(double x) {
  if (x <= -1.0 || x >= 1.0) return 0.0;
  return 0.42 + 0.5 * std::cos(kPi * x) + 0.08 * std::cos(2.0 * kPi * x);
}

}  // namespace

Resampler::Resampler(double inRate, double outRate, bool trimmable)
    : baseStep_(inRate / outRate),
      step_(inRate / outRate),
      passthrough_(inRate == outRate && !trimmable),
      position_(kHalfTaps),
      history_(kHalfTaps, 0.0f) {
  if (passthrough_) {
    return;
  }
  // Cut off just under the lower of the two Nyquist frequencies, so
  // downsampling (e.g. 96 kHz -> 48 kHz) cannot alias.
  const double cutoff = std::min(1.0, outRate / inRate) * 0.95;
  table_.resize(static_cast<size_t>(kPhases + 1) * 2 * kHalfTaps);
  for (int p = 0; p <= kPhases; p++) {
    const double frac = static_cast<double>(p) / kPhases;
    for (int j = 0; j < 2 * kHalfTaps; j++) {
      // Tap j multiplies input sample floor(position) - kHalfTaps + 1 + j.
      const double t = static_cast<double>(j - kHalfTaps + 1) - frac;
      const double x = cutoff * t;
      const double sinc = x == 0.0 ? 1.0 : std::sin(kPi * x) / (kPi * x);
      table_[static_cast<size_t>(p) * 2 * kHalfTaps + j] = static_cast<float>(cutoff * sinc * Window(t / kHalfTaps));
    }
  }
}

float Resampler::Interpolate(double position) const {
  const size_t base = static_cast<size_t>(position);
  const double phase = (position - static_cast<double>(base)) * kPhases;
  const size_t p = static_cast<size_t>(phase);
  const float mix = static_cast<float>(phase - static_cast<double>(p));
  const float* lo = &table_[p * 2 * kHalfTaps];
  const float* hi = lo + 2 * kHalfTaps;
  const float* x = &history_[base - kHalfTaps + 1];
  float acc = 0.0f;
  for (int j = 0; j < 2 * kHalfTaps; j++) {
    acc += x[j] * (lo[j] + (hi[j] - lo[j]) * mix);
  }
  return acc;
}

void Resampler::Process(const float* in, size_t n, std::vector<float>& out) {
  if (passthrough_) {
    out.insert(out.end(), in, in + n);
    produced_ += n;
    return;
  }
  history_.insert(history_.end(), in, in + n);
  while (static_cast<size_t>(position_) + kHalfTaps < history_.size()) {
    out.push_back(Interpolate(position_));
    position_ += step_;
    produced_++;
  }
  // Keep only what the next interpolation still needs to look back on.
  const size_t keepFrom = static_cast<size_t>(position_) + 1 - kHalfTaps;
  history_.erase(history_.begin(), history_.begin() + static_cast<std::ptrdiff_t>(keepFrom));
  position_ -= static_cast<double>(keepFrom);
}

void Resampler::SetRatioTrim(double trim) {
  if (!passthrough_) {
    step_ = baseStep_ * (1.0 + trim);
  }
}

double Resampler::NextInputOutputPosition() const {
  if (passthrough_) {
    return static_cast<double>(produced_);
  }
  // The next input lands at history_.size(); outputs still owed before it are
  // (that index - position_) / step_ away from the next output.
  return static_cast<double>(produced_) + (static_cast<double>(history_.size()) - position_) / step_;
}

}  // namespace sayso::aec
