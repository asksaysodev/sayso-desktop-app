// Converts whatever interleaved format a device delivers into the mono float
// the echo canceller works in. The format comes from the device at runtime;
// nothing here assumes a sample rate, channel count or sample type.

#ifndef SAYSO_AEC_AUDIO_FORMAT_H_
#define SAYSO_AEC_AUDIO_FORMAT_H_

#include <cstddef>
#include <cstdint>

namespace sayso::aec {

enum class SampleType { kFloat32, kInt16, kInt24, kInt32, kUnsupported };

SampleType DescribeSampleType(int bitDepth, bool isFloat);

// Writes `frames` mono samples in [-1, 1] to `dst`: the average of every
// channel, so the mapping is the same whatever the device's channel order.
// A null `src` (a packet the device flagged silent) writes zeros.
void DownmixToMono(const uint8_t* src, size_t frames, int channels, SampleType type, float* dst);

}  // namespace sayso::aec

#endif  // SAYSO_AEC_AUDIO_FORMAT_H_
