#include "audio_format.h"

#include <cstring>

namespace sayso::aec {

SampleType DescribeSampleType(int bitDepth, bool isFloat) {
  if (isFloat) {
    return bitDepth == 32 ? SampleType::kFloat32 : SampleType::kUnsupported;
  }
  switch (bitDepth) {
    case 16: return SampleType::kInt16;
    case 24: return SampleType::kInt24;
    case 32: return SampleType::kInt32;
    default: return SampleType::kUnsupported;
  }
}

namespace {

inline float ReadSample(const uint8_t* p, SampleType type) {
  switch (type) {
    case SampleType::kFloat32: {
      float v;
      memcpy(&v, p, sizeof(v));
      return v;
    }
    case SampleType::kInt16: {
      int16_t v;
      memcpy(&v, p, sizeof(v));
      return static_cast<float>(v) / 32768.0f;
    }
    case SampleType::kInt24: {
      // Little-endian, sign-extended from the top byte.
      const int32_t v = static_cast<int32_t>(static_cast<uint32_t>(p[0]) << 8 | static_cast<uint32_t>(p[1]) << 16 |
                                             static_cast<uint32_t>(p[2]) << 24) >> 8;
      return static_cast<float>(v) / 8388608.0f;
    }
    case SampleType::kInt32: {
      int32_t v;
      memcpy(&v, p, sizeof(v));
      return static_cast<float>(v) / 2147483648.0f;
    }
    default:
      return 0.0f;
  }
}

inline int BytesPerSample(SampleType type) {
  switch (type) {
    case SampleType::kInt16: return 2;
    case SampleType::kInt24: return 3;
    case SampleType::kFloat32:
    case SampleType::kInt32: return 4;
    default: return 0;
  }
}

}  // namespace

void DownmixToMono(const uint8_t* src, size_t frames, int channels, SampleType type, float* dst) {
  const int bytes = BytesPerSample(type);
  if (!src || bytes == 0 || channels <= 0) {
    memset(dst, 0, frames * sizeof(float));
    return;
  }
  const float scale = 1.0f / static_cast<float>(channels);
  const size_t stride = static_cast<size_t>(bytes) * channels;
  for (size_t f = 0; f < frames; f++) {
    const uint8_t* frame = src + f * stride;
    float sum = 0.0f;
    for (int c = 0; c < channels; c++) {
      sum += ReadSample(frame + c * bytes, type);
    }
    dst[f] = sum * scale;
  }
}

}  // namespace sayso::aec
