#include "diagnostic_recorder.h"

#include <algorithm>
#include <cmath>
#include <filesystem>

namespace sayso::aec {

namespace {

constexpr const char* kSuffixes[] = {"-reference.wav", "-mic_raw.wav", "-mic_clean.wav"};
// Blocks waiting for the disk beyond this are dropped (and counted) rather
// than letting memory grow behind a stalled disk. ~30 s of audio.
constexpr size_t kMaxQueuedBlocks = 30;

void Put16(std::ofstream& f, uint16_t v) { f.write(reinterpret_cast<const char*>(&v), 2); }
void Put32(std::ofstream& f, uint32_t v) { f.write(reinterpret_cast<const char*>(&v), 4); }

int16_t ToPcm16(float s) {
  const float clamped = std::max(-1.0f, std::min(1.0f, s));
  return static_cast<int16_t>(std::lround(clamped * 32767.0f));
}

}  // namespace

DiagnosticRecorder::~DiagnosticRecorder() { Close(); }

void DiagnosticRecorder::WriteHeader(std::ofstream& f, int sampleRate, uint32_t dataBytes) {
  f.seekp(0);
  f.write("RIFF", 4);
  Put32(f, 36 + dataBytes);
  f.write("WAVEfmt ", 8);
  Put32(f, 16);                                   // fmt chunk size
  Put16(f, 1);                                    // PCM
  Put16(f, 1);                                    // mono
  Put32(f, static_cast<uint32_t>(sampleRate));
  Put32(f, static_cast<uint32_t>(sampleRate) * 2);  // byte rate
  Put16(f, 2);                                    // block align
  Put16(f, 16);                                   // bits per sample
  f.write("data", 4);
  Put32(f, dataBytes);
}

bool DiagnosticRecorder::Open(const std::string& folderUtf8, const std::string& stem, int sampleRate,
                              double maxSeconds, std::string* error) {
  Close();
  // u8path keeps non-ASCII folder names intact on Windows.
  const std::filesystem::path folder(std::u8string(folderUtf8.begin(), folderUtf8.end()));
  std::error_code ec;
  if (!std::filesystem::is_directory(folder, ec)) {
    *error = "folder_missing";
    return false;
  }
  for (int c = 0; c < kChannels; c++) {
    const std::string name = stem + kSuffixes[c];
    files_[c].open(folder / std::u8string(name.begin(), name.end()), std::ios::binary | std::ios::trunc);
    if (!files_[c].is_open()) {
      for (int k = 0; k < kChannels; k++) files_[k].close();
      *error = "open_failed";
      return false;
    }
    WriteHeader(files_[c], sampleRate, 0);
  }
  sampleRate_ = sampleRate;
  maxSamples_ = static_cast<uint64_t>(std::max(1.0, maxSeconds) * sampleRate);
  samplesWritten_ = 0;
  droppedBlocks_ = 0;
  limitReached_ = false;
  stem_ = stem;
  for (auto& s : pending_.samples) s.clear();
  {
    std::lock_guard<std::mutex> lk(mutex_);
    queue_.clear();
    closing_ = false;
  }
  writer_ = std::thread([this] { WriterMain(); });
  open_ = true;
  return true;
}

bool DiagnosticRecorder::WriteFrame(const float* reference, const float* micRaw, const float* micClean,
                                    size_t samples) {
  if (!open_ || limitReached_) {
    return false;
  }
  const size_t n = static_cast<size_t>(std::min<uint64_t>(samples, maxSamples_ - samplesWritten_));
  const float* sources[kChannels] = {reference, micRaw, micClean};
  for (int c = 0; c < kChannels; c++) {
    for (size_t i = 0; i < n; i++) {
      pending_.samples[c].push_back(ToPcm16(sources[c][i]));
    }
  }
  samplesWritten_ += n;
  if (pending_.samples[0].size() >= static_cast<size_t>(sampleRate_)) {
    PushPending();
  }
  if (samplesWritten_ >= maxSamples_) {
    limitReached_ = true;
    PushPending();
    return false;
  }
  return true;
}

void DiagnosticRecorder::PushPending() {
  if (pending_.samples[0].empty()) {
    return;
  }
  {
    std::lock_guard<std::mutex> lk(mutex_);
    if (queue_.size() >= kMaxQueuedBlocks) {
      droppedBlocks_++;
    } else {
      queue_.push_back(std::move(pending_));
    }
  }
  pending_ = Block();
  cv_.notify_one();
}

void DiagnosticRecorder::WriterMain() {
  uint64_t bytes = 0;  // per file
  for (;;) {
    Block block;
    {
      std::unique_lock<std::mutex> lk(mutex_);
      cv_.wait(lk, [this] { return closing_ || !queue_.empty(); });
      if (queue_.empty()) {
        break;  // closing and drained
      }
      block = std::move(queue_.front());
      queue_.pop_front();
    }
    const size_t blockBytes = block.samples[0].size() * sizeof(int16_t);
    for (int c = 0; c < kChannels; c++) {
      files_[c].seekp(0, std::ios::end);
      files_[c].write(reinterpret_cast<const char*>(block.samples[c].data()),
                      static_cast<std::streamsize>(blockBytes));
    }
    bytes += blockBytes;
    // Keep the header current, so a crash still leaves playable files.
    for (int c = 0; c < kChannels; c++) {
      WriteHeader(files_[c], sampleRate_, static_cast<uint32_t>(bytes));
      files_[c].flush();
    }
  }
}

void DiagnosticRecorder::Close() {
  if (!open_) {
    return;
  }
  PushPending();
  {
    std::lock_guard<std::mutex> lk(mutex_);
    closing_ = true;
  }
  cv_.notify_one();
  writer_.join();
  for (int c = 0; c < kChannels; c++) {
    files_[c].close();
  }
  open_ = false;
}

double DiagnosticRecorder::SecondsWritten() const {
  return sampleRate_ > 0 ? static_cast<double>(samplesWritten_) / sampleRate_ : 0.0;
}

}  // namespace sayso::aec
