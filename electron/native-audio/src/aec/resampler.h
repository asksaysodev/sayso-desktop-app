// Streaming resampler from a device's rate to the echo canceller's rate.
//
// Windowed-sinc interpolation at an arbitrary ratio: device rates are read at
// runtime (44.1 kHz, 48 kHz, 96 kHz, ...), and the ratio can later be nudged
// a few ppm to correct clock drift between mic and speakers. A device already
// at the target rate is copied through untouched.
//
// Output sample k corresponds to input sample position k * (inRate / outRate)
// of the same stream, so timing computed on the input side carries over.

#ifndef SAYSO_AEC_RESAMPLER_H_
#define SAYSO_AEC_RESAMPLER_H_

#include <cstddef>
#include <cstdint>
#include <vector>

namespace sayso::aec {

class Resampler {
 public:
  // `trimmable`: the ratio can be nudged with SetRatioTrim (used to make one
  // stream follow another's clock). A trimmable resampler always
  // interpolates, even between equal rates, so a trim never causes a jump.
  Resampler(double inRate, double outRate, bool trimmable = false);

  // Appends the output produced by `n` more input samples to `out`. The
  // interpolator looks a few samples ahead, so output trails input by about
  // kHalfTaps input samples until the stream ends.
  void Process(const float* in, size_t n, std::vector<float>& out);

  // Input samples consumed per output sample (trim included).
  double Step() const { return step_; }

  // Runs the conversion `trim` faster (> 0: consumes input faster, so fewer
  // output samples) or slower (< 0), e.g. 100e-6 = 100 ppm. Trimmable only.
  void SetRatioTrim(double trim);

  // Output index at which the next input sample (the first of the next
  // Process call) will appear, counting every output produced so far.
  double NextInputOutputPosition() const;

 private:
  static constexpr int kHalfTaps = 16;  // taps either side of the interpolation point
  static constexpr int kPhases = 256;   // kernel table resolution between input samples

  float Interpolate(double position) const;

  const double baseStep_;
  double step_;
  const bool passthrough_;
  uint64_t produced_ = 0;        // output samples produced so far
  double position_;              // next output's position, as an index into history_
  std::vector<float> history_;   // unconsumed input plus kHalfTaps of look-back
  std::vector<float> table_;     // (kPhases + 1) rows of 2 * kHalfTaps coefficients
};

}  // namespace sayso::aec

#endif  // SAYSO_AEC_RESAMPLER_H_
