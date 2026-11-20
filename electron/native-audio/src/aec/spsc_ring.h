// Single-producer / single-consumer ring buffer. Lock-free, and allocates
// nothing after construction, so a capture thread can write into it without
// ever waiting on the echo-cancellation worker that reads from it.

#ifndef SAYSO_AEC_SPSC_RING_H_
#define SAYSO_AEC_SPSC_RING_H_

#include <atomic>
#include <cstddef>
#include <vector>

namespace sayso::aec {

template <typename T>
class SpscRing {
 public:
  // Capacity rounds up to a power of two.
  explicit SpscRing(size_t capacity) : buffer_(RoundUpPow2(capacity)), mask_(buffer_.size() - 1) {}

  SpscRing(const SpscRing&) = delete;
  SpscRing& operator=(const SpscRing&) = delete;

  // Producer only. Free space only grows while the producer is not writing,
  // so a FreeSpace() check followed by Write() is safe.
  size_t FreeSpace() const {
    return buffer_.size() - (head_.load(std::memory_order_relaxed) - tail_.load(std::memory_order_acquire));
  }

  // Producer only. All or nothing: returns false, writing nothing, when `n`
  // items do not fit.
  bool Write(const T* src, size_t n) {
    const size_t head = head_.load(std::memory_order_relaxed);
    if (buffer_.size() - (head - tail_.load(std::memory_order_acquire)) < n) {
      return false;
    }
    for (size_t i = 0; i < n; i++) {
      buffer_[(head + i) & mask_] = src[i];
    }
    head_.store(head + n, std::memory_order_release);
    return true;
  }

  // Producer only: running count of items ever written, i.e. where the next
  // Write() lands. Lets a consumer match records in one ring to items in another.
  size_t WritePosition() const { return head_.load(std::memory_order_relaxed); }

  // Consumer only: running count of items ever consumed.
  size_t ReadPosition() const { return tail_.load(std::memory_order_relaxed); }

  // Consumer only. Discards `n` readable items; the caller checked Available().
  void Skip(size_t n) { tail_.store(tail_.load(std::memory_order_relaxed) + n, std::memory_order_release); }

  // Consumer only.
  size_t Available() const {
    return head_.load(std::memory_order_acquire) - tail_.load(std::memory_order_relaxed);
  }

  // Consumer only. Reads exactly `n` items; the caller checked Available().
  void Read(T* dst, size_t n) {
    const size_t tail = tail_.load(std::memory_order_relaxed);
    for (size_t i = 0; i < n; i++) {
      dst[i] = buffer_[(tail + i) & mask_];
    }
    tail_.store(tail + n, std::memory_order_release);
  }

  // Consumer only. Discards everything currently readable.
  void Clear() { tail_.store(head_.load(std::memory_order_acquire), std::memory_order_release); }

 private:
  static size_t RoundUpPow2(size_t n) {
    size_t p = 1;
    while (p < n) p <<= 1;
    return p;
  }

  std::vector<T> buffer_;
  const size_t mask_;
  alignas(64) std::atomic<size_t> head_{0};  // written by the producer
  alignas(64) std::atomic<size_t> tail_{0};  // written by the consumer
};

}  // namespace sayso::aec

#endif  // SAYSO_AEC_SPSC_RING_H_
