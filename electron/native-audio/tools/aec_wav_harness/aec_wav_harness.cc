// Offline harness: runs recorded audio through the same AecPipeline and
// engine the addon uses, without devices or Electron.
//
//   aec_wav_harness <reference.wav> <mic.wav> <clean_out.wav> [ref_offset_ms] [jitter_ms]
//
// reference.wav = what the computer played (system audio)
// mic.wav       = what the microphone heard, recorded at the same time
// clean_out.wav = echo-cancelled mic, 48 kHz mono 16-bit
//
// Both inputs are fed as 10 ms packets stamped with the same simulated host
// clock, as WASAPI would, in whatever rate/channels/format the files use.
// ref_offset_ms = how much later the reference recording started than the mic
// recording (default 0). Recordings made without shared timestamps rarely
// start together; a cross-correlation of the two files gives the offset.
// Real capture needs no such value: WASAPI's QPC timestamps carry it.
// jitter_ms = random +/- error added to every packet timestamp, as remote and
// virtual devices produce; output quality should not depend on it.
// Prints the pipeline's lifecycle events, then mic vs clean level per second:
// where only the far end played, clean should sit far below mic.

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <mutex>
#include <random>
#include <string>
#include <thread>
#include <vector>

#include "../../src/aec/aec_pipeline.h"

using sayso::aec::AecPath;
using sayso::aec::AecPipeline;
using sayso::aec::SampleType;

namespace {

struct Wav {
  int rate = 0;
  int channels = 0;
  int bits = 0;
  bool isFloat = false;
  std::vector<uint8_t> data;
  size_t Frames() const { return data.size() / (static_cast<size_t>(channels) * (bits / 8)); }
};

bool ReadWav(const char* path, Wav* wav) {
  std::ifstream f(path, std::ios::binary);
  std::vector<uint8_t> d((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
  if (d.size() < 12 || memcmp(d.data(), "RIFF", 4) != 0 || memcmp(d.data() + 8, "WAVE", 4) != 0) return false;
  size_t i = 12;
  bool haveFmt = false;
  while (i + 8 <= d.size()) {
    uint32_t size;
    memcpy(&size, &d[i + 4], 4);
    if (memcmp(&d[i], "fmt ", 4) == 0) {
      uint16_t tag, ch, bits;
      uint32_t rate;
      memcpy(&tag, &d[i + 8], 2);
      memcpy(&ch, &d[i + 10], 2);
      memcpy(&rate, &d[i + 12], 4);
      memcpy(&bits, &d[i + 22], 2);
      if (tag == 0xFFFE && size >= 40) {
        memcpy(&tag, &d[i + 8 + 24], 2);  // first two bytes of the SubFormat GUID
      }
      wav->rate = static_cast<int>(rate);
      wav->channels = ch;
      wav->bits = bits;
      wav->isFloat = tag == 3;
      haveFmt = true;
    } else if (memcmp(&d[i], "data", 4) == 0 && haveFmt) {
      const size_t n = std::min<size_t>(size, d.size() - (i + 8));
      wav->data.assign(d.begin() + static_cast<std::ptrdiff_t>(i + 8),
                       d.begin() + static_cast<std::ptrdiff_t>(i + 8 + n));
      return true;
    }
    i += 8 + size + (size & 1);
  }
  return false;
}

void WriteWav16(const char* path, const std::vector<float>& samples, int rate) {
  std::ofstream f(path, std::ios::binary);
  const uint32_t dataBytes = static_cast<uint32_t>(samples.size() * 2);
  const uint32_t riff = 36 + dataBytes, fmtSize = 16, byteRate = static_cast<uint32_t>(rate) * 2, srate = rate;
  const uint16_t pcm = 1, ch = 1, align = 2, bits = 16;
  f.write("RIFF", 4); f.write(reinterpret_cast<const char*>(&riff), 4); f.write("WAVEfmt ", 8);
  f.write(reinterpret_cast<const char*>(&fmtSize), 4); f.write(reinterpret_cast<const char*>(&pcm), 2);
  f.write(reinterpret_cast<const char*>(&ch), 2); f.write(reinterpret_cast<const char*>(&srate), 4);
  f.write(reinterpret_cast<const char*>(&byteRate), 4); f.write(reinterpret_cast<const char*>(&align), 2);
  f.write(reinterpret_cast<const char*>(&bits), 2); f.write("data", 4);
  f.write(reinterpret_cast<const char*>(&dataBytes), 4);
  for (float s : samples) {
    const int16_t v = static_cast<int16_t>(std::lround(std::max(-1.0f, std::min(1.0f, s)) * 32767.0f));
    f.write(reinterpret_cast<const char*>(&v), 2);
  }
}

std::atomic<uint64_t> g_simNow{0};
uint64_t SimNow() { return g_simNow.load(); }
void Print(const std::string& e) { printf("[event] %s\n", e.c_str()); }

double Db(double sumSquares, size_t n) { return n ? 10.0 * std::log10(sumSquares / n + 1e-12) : -120.0; }

}  // namespace

int main(int argc, char** argv) {
  setvbuf(stdout, nullptr, _IONBF, 0);  // events and results show even if the run dies
  if (argc < 4 || argc > 6) {
    fprintf(stderr, "usage: aec_wav_harness <reference.wav> <mic.wav> <clean_out.wav> [ref_offset_ms] [jitter_ms]\n");
    return 2;
  }
  const double refOffsetMs = argc >= 5 ? atof(argv[4]) : 0.0;
  const double jitterMs = argc >= 6 ? atof(argv[5]) : 0.0;
  std::mt19937 rng(1);
  std::uniform_real_distribution<double> jitter(-jitterMs * 10000.0, jitterMs * 10000.0);
  auto jittered = [&](uint64_t t) { return t + static_cast<uint64_t>(static_cast<int64_t>(jitter(rng))); };
  Wav ref, mic;
  if (!ReadWav(argv[1], &ref) || !ReadWav(argv[2], &mic)) {
    fprintf(stderr, "could not read input WAVs\n");
    return 1;
  }
  printf("reference: %d Hz, %d ch, %d-bit%s, %.2f s\n", ref.rate, ref.channels, ref.bits, ref.isFloat ? " float" : "",
         static_cast<double>(ref.Frames()) / ref.rate);
  printf("mic:       %d Hz, %d ch, %d-bit%s, %.2f s\n", mic.rate, mic.channels, mic.bits, mic.isFloat ? " float" : "",
         static_cast<double>(mic.Frames()) / mic.rate);

  std::vector<float> clean;
  std::mutex cleanMutex;
  AecPipeline pipeline(Print, SimNow);
  pipeline.SetCleanSink([&](const float* frame, size_t n) {
    std::lock_guard<std::mutex> lk(cleanMutex);
    clean.insert(clean.end(), frame, frame + n);
  });
  // Same switch as the addon: SAYSO_AEC_DIAG_WAV_DIR records the three
  // aligned WAVs of this run there.
  if (const char* dir = std::getenv("SAYSO_AEC_DIAG_WAV_DIR")) {
    pipeline.SetDiagnosticRecording(dir, 300);
  }
  pipeline.Start();

  // Simulated QPC: both streams start at the same instant, 100 ns units.
  const uint64_t t0 = 10'000'000'000ull;
  const uint64_t refT0 = t0 + static_cast<uint64_t>(static_cast<int64_t>(refOffsetMs * 10000.0));
  printf("reference offset: %+.1f ms, timestamp jitter: +/-%.1f ms\n", refOffsetMs, jitterMs);
  const SampleType refType = sayso::aec::DescribeSampleType(ref.bits, ref.isFloat);
  const SampleType micType = sayso::aec::DescribeSampleType(mic.bits, mic.isFloat);
  const size_t refPacket = static_cast<size_t>(ref.rate / 100), micPacket = static_cast<size_t>(mic.rate / 100);
  const size_t refStride = static_cast<size_t>(ref.channels) * (ref.bits / 8);
  const size_t micStride = static_cast<size_t>(mic.channels) * (mic.bits / 8);
  size_t refPos = 0, micPos = 0;
  for (int packet = 0; refPos < ref.Frames() || micPos < mic.Frames(); packet++) {
    const uint64_t host = t0 + static_cast<uint64_t>(packet) * 100000;  // 10 ms per packet
    // Packets "arrive" 20 ms after their true capture time, as from a real device.
    g_simNow.store(host + 200'000);
    if (refPos < ref.Frames()) {
      const uint32_t n = static_cast<uint32_t>(std::min(refPacket, ref.Frames() - refPos));
      const uint64_t refHost = refT0 + static_cast<uint64_t>(static_cast<double>(refPos) * 1e7 / ref.rate);
      pipeline.PushPacket(AecPath::kRender, 1, ref.rate, &ref.data[refPos * refStride], n, ref.channels, refType,
                          jittered(refHost), true);
      refPos += n;
    }
    if (micPos < mic.Frames()) {
      const uint32_t n = static_cast<uint32_t>(std::min(micPacket, mic.Frames() - micPos));
      const uint64_t micHost = t0 + static_cast<uint64_t>(static_cast<double>(micPos) * 1e7 / mic.rate);
      pipeline.PushPacket(AecPath::kCapture, 2, mic.rate, &mic.data[micPos * micStride], n, mic.channels, micType,
                          jittered(micHost), true);
      micPos += n;
    }
    if (packet % 20 == 19) {
      std::this_thread::sleep_for(std::chrono::milliseconds(15));  // let the worker keep up; rings hold ~1 s
    }
  }
  g_simNow.store(g_simNow.load() + 10'000'000);  // let the last frames stop waiting for reference
  std::this_thread::sleep_for(std::chrono::milliseconds(300));
  pipeline.Stop();

  WriteWav16(argv[3], clean, AecPipeline::kProcessRate);

  // Mic vs clean level per second, both at 48 kHz on the same timeline.
  std::vector<float> micMono(mic.Frames());
  sayso::aec::DownmixToMono(mic.data.data(), mic.Frames(), mic.channels, micType, micMono.data());
  printf("\nsecond  mic_dBFS  clean_dBFS  reduction_dB\n");
  double totalReduction = 0;
  int counted = 0;
  const size_t seconds = std::min(clean.size() / AecPipeline::kProcessRate, micMono.size() / mic.rate);
  for (size_t s = 0; s < seconds; s++) {
    double m = 0, c = 0;
    for (size_t i = 0; i < static_cast<size_t>(mic.rate); i++) m += micMono[s * mic.rate + i] * micMono[s * mic.rate + i];
    for (size_t i = 0; i < static_cast<size_t>(AecPipeline::kProcessRate); i++) {
      const float v = clean[s * AecPipeline::kProcessRate + i];
      c += v * v;
    }
    const double md = Db(m, mic.rate), cd = Db(c, AecPipeline::kProcessRate);
    printf("%6zu  %8.1f  %10.1f  %12.1f\n", s, md, cd, md - cd);
    totalReduction += md - cd;
    counted++;
  }
  printf("\nclean: %zu samples (%.2f s) -> %s; mean reduction %.1f dB\n", clean.size(),
         static_cast<double>(clean.size()) / AecPipeline::kProcessRate, argv[3],
         counted ? totalReduction / counted : 0.0);
  return 0;
}
