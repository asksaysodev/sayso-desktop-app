#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <memory>

namespace sayso::echo {

class ChunkRing {
 public:
  static constexpr size_t kSlotSamples = 4096;
  static constexpr uint32_t kMaxChannels = 8;

  struct Slot {
    uint32_t frames = 0;
    uint32_t channels = 0;
    int32_t rate = 0;
    uint64_t ns = 0;
    float data[kSlotSamples];
  };

  explicit ChunkRing(size_t slots) {
    size_t cap = 2;
    while (cap < slots) cap <<= 1;
    cap_ = cap;
    slots_.reset(new Slot[cap]);
  }

  size_t capacity() const { return cap_; }
  size_t size() const { return tail_.load(std::memory_order_acquire) - head_.load(std::memory_order_acquire); }

  bool Push(const float* interleaved, size_t frames, int channels, int rate, uint64_t ns) {
    return PushImpl(interleaved, frames, channels, rate, ns);
  }
  bool Push(const int16_t* interleaved, size_t frames, int channels, int rate, uint64_t ns) {
    return PushImpl(interleaved, frames, channels, rate, ns);
  }

  const Slot* Front() const {
    const size_t head = head_.load(std::memory_order_relaxed);
    if (head == tail_.load(std::memory_order_acquire)) return nullptr;
    return &slots_[head & (cap_ - 1)];
  }
  void Pop() { head_.store(head_.load(std::memory_order_relaxed) + 1, std::memory_order_release); }

  void Clear() { head_.store(tail_.load(std::memory_order_acquire), std::memory_order_release); }

 private:
  static float ToFloat(float v) { return v; }
  static float ToFloat(int16_t v) { return static_cast<float>(v) * (1.f / 32768.f); }

  template <typename T>
  bool PushImpl(const T* interleaved, size_t frames, int channels, int rate, uint64_t ns) {
    if (!interleaved || frames == 0 || channels < 1 || channels > static_cast<int>(kMaxChannels) || rate <= 0) return false;
    const size_t perSlot = kSlotSamples / static_cast<size_t>(channels);
    const size_t need = (frames + perSlot - 1) / perSlot;
    const size_t tail = tail_.load(std::memory_order_relaxed);
    if (cap_ - (tail - head_.load(std::memory_order_acquire)) < need) return false;
    size_t done = 0;
    for (size_t i = 0; i < need; i++) {
      Slot& s = slots_[(tail + i) & (cap_ - 1)];
      const size_t n = frames - done < perSlot ? frames - done : perSlot;
      s.frames = static_cast<uint32_t>(n);
      s.channels = static_cast<uint32_t>(channels);
      s.rate = rate;
      s.ns = ns + static_cast<uint64_t>(static_cast<double>(done) / static_cast<double>(rate) * 1e9);
      const T* src = interleaved + done * static_cast<size_t>(channels);
      for (size_t k = 0; k < n * static_cast<size_t>(channels); k++) s.data[k] = ToFloat(src[k]);
      done += n;
    }
    tail_.store(tail + need, std::memory_order_release);
    return true;
  }

  size_t cap_ = 0;
  std::unique_ptr<Slot[]> slots_;
  alignas(64) std::atomic<size_t> head_{0};
  alignas(64) std::atomic<size_t> tail_{0};
};

}
