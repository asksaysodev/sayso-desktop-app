#pragma once

#include <atomic>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace sayso::echo {

struct StampStats {
  uint64_t buffers = 0;
  uint64_t measured = 0;
  uint64_t estimated = 0;
  uint64_t nonMonotonic = 0;
  uint64_t gaps = 0;
  uint64_t rebases = 0;
  double sampleRate = 0;
  double lagMs = 0;
  double jitterUs = 0;
  double maxJitterUs = 0;
  double clockPpm = 0;
  bool clockPpmValid = false;
};

class StampTracker {
 public:
  static constexpr double kGapNs = 20e6;
  static constexpr double kOffsetAlpha = 0.02;
  static constexpr double kJitterAlpha = 0.05;
  static constexpr double kWarmupNs = 1e9;
  static constexpr double kMinDriftSpanNs = 3e9;

  void Reset() {
    buffers_ = measured_ = estimated_ = nonMonotonic_ = gaps_ = rebases_ = 0;
    sampleRate_ = 0; lagMs_ = 0; jitterUs_ = 0; maxJitterUs_ = 0; clockPpm_ = 0;
    clockPpmValid_ = false;
    Rebase();
  }

  void RequestReset() { resetRequested_.store(true, std::memory_order_release); }

  void Rebase() {
    haveBase_ = false;
    frames_ = 0;
    havePrev_ = false;
    clockPpmValid_ = false;
    maxJitterUs_ = 0;
    haveAnchor_ = false;
  }

  void Observe(uint64_t stampNs, size_t frames, double sampleRate, bool measured, uint64_t callbackNs) {
    if (resetRequested_.exchange(false, std::memory_order_acq_rel)) Reset();
    if (frames == 0 || sampleRate <= 0) return;
    buffers_++;
    (measured ? measured_ : estimated_)++;
    if (sampleRate != sampleRate_.load(std::memory_order_relaxed)) {
      if (sampleRate_.load(std::memory_order_relaxed) != 0) rebases_++;
      sampleRate_ = sampleRate;
      Rebase();
    }
    const double stamp = static_cast<double>(stampNs);
    const double durNs = static_cast<double>(frames) / sampleRate * 1e9;
    lagMs_ = (static_cast<double>(callbackNs) - (stamp + durNs)) / 1e6;

    if (havePrev_ && stampNs <= prevStamp_) nonMonotonic_++;
    prevStamp_ = stampNs;
    havePrev_ = true;

    const double offset = stamp - static_cast<double>(frames_) / sampleRate * 1e9;
    if (!haveBase_) {
      mean_ = offset;
      meanStamp_ = stamp;
      firstStamp_ = stamp;
      haveBase_ = true;
      jitter_ = 0;
    } else {
      const double dev = offset - mean_;
      if (dev > kGapNs) {
        gaps_++;
        rebases_++;
        Rebase();
        havePrev_ = true;
        haveBase_ = true;
        frames_ = 0;
        mean_ = meanStamp_ = firstStamp_ = stamp;
        jitter_ = 0;
      } else {
        jitter_ += kJitterAlpha * (std::fabs(dev) - jitter_);
        if (std::fabs(dev) / 1e3 > maxJitterUs_) maxJitterUs_ = std::fabs(dev) / 1e3;
        mean_ += kOffsetAlpha * dev;
        meanStamp_ += kOffsetAlpha * (stamp - meanStamp_);
        if (!haveAnchor_ && stamp - firstStamp_ >= kWarmupNs) {
          haveAnchor_ = true;
          anchorOffset_ = mean_;
          anchorStamp_ = meanStamp_;
        } else if (haveAnchor_ && meanStamp_ - anchorStamp_ >= kMinDriftSpanNs) {
          clockPpm_ = -(mean_ - anchorOffset_) / (meanStamp_ - anchorStamp_) * 1e6;
          clockPpmValid_ = true;
        }
      }
    }
    jitterUs_ = jitter_ / 1e3;
    frames_ += frames;
  }

  StampStats Get() const {
    StampStats s;
    s.buffers = buffers_; s.measured = measured_; s.estimated = estimated_;
    s.nonMonotonic = nonMonotonic_; s.gaps = gaps_; s.rebases = rebases_;
    s.sampleRate = sampleRate_; s.lagMs = lagMs_; s.jitterUs = jitterUs_; s.maxJitterUs = maxJitterUs_;
    s.clockPpm = clockPpm_; s.clockPpmValid = clockPpmValid_;
    return s;
  }

 private:
  std::atomic<uint64_t> buffers_{0}, measured_{0}, estimated_{0}, nonMonotonic_{0}, gaps_{0}, rebases_{0};
  std::atomic<double> sampleRate_{0}, lagMs_{0}, jitterUs_{0}, maxJitterUs_{0}, clockPpm_{0};
  std::atomic<bool> clockPpmValid_{false};
  std::atomic<bool> resetRequested_{false};
  bool haveBase_ = false, havePrev_ = false, haveAnchor_ = false;
  uint64_t frames_ = 0;
  uint64_t prevStamp_ = 0;
  double mean_ = 0, meanStamp_ = 0, firstStamp_ = 0, anchorOffset_ = 0, anchorStamp_ = 0, jitter_ = 0;
};

}
