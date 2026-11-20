// Opt-in diagnostic recording of what the echo canceller saw and produced:
// the system-audio reference, the raw mic and the cleaned mic, as three
// sample-aligned WAV files (mono 16-bit at the processing rate). Comparing
// them shows whether the far end was removed and the near end kept.
//
// Never on by default: the pipeline only opens one when the platform layer
// passes a folder. Bounded by a maximum duration; the files are the user's to
// delete. Disk writes run on the recorder's own thread, so a slow disk can
// delay the recording but never the audio.

#ifndef SAYSO_AEC_DIAGNOSTIC_RECORDER_H_
#define SAYSO_AEC_DIAGNOSTIC_RECORDER_H_

#include <condition_variable>
#include <cstdint>
#include <deque>
#include <fstream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace sayso::aec {

class DiagnosticRecorder {
 public:
  DiagnosticRecorder() = default;
  ~DiagnosticRecorder();
  DiagnosticRecorder(const DiagnosticRecorder&) = delete;
  DiagnosticRecorder& operator=(const DiagnosticRecorder&) = delete;

  // Creates <folder>/<stem>-reference.wav, -mic_raw.wav and -mic_clean.wav.
  // `folderUtf8` must exist. On failure returns false with `error` set and
  // leaves nothing open.
  bool Open(const std::string& folderUtf8, const std::string& stem, int sampleRate, double maxSeconds,
            std::string* error);

  bool IsOpen() const { return open_; }

  // One frame of each signal, same instant, same length. Returns false once
  // the duration limit is reached; nothing more is recorded after that.
  bool WriteFrame(const float* reference, const float* micRaw, const float* micClean, size_t samples);

  // Flushes, finalizes the headers and closes the files. Safe to call twice.
  void Close();

  double SecondsWritten() const;
  uint64_t DroppedBlocks() const { return droppedBlocks_; }
  bool LimitReached() const { return limitReached_; }
  const std::string& Stem() const { return stem_; }

 private:
  static constexpr int kChannels = 3;  // reference, mic_raw, mic_clean

  struct Block {
    std::vector<int16_t> samples[kChannels];
  };

  void WriterMain();
  void PushPending();
  static void WriteHeader(std::ofstream& f, int sampleRate, uint32_t dataBytes);

  bool open_ = false;
  bool limitReached_ = false;
  int sampleRate_ = 0;
  uint64_t maxSamples_ = 0;
  uint64_t samplesWritten_ = 0;  // per file; all three advance together
  uint64_t droppedBlocks_ = 0;
  std::string stem_;
  Block pending_;                // filled by the caller's thread, about 1 s at a time

  std::ofstream files_[kChannels];
  std::thread writer_;
  std::mutex mutex_;
  std::condition_variable cv_;
  std::deque<Block> queue_;      // guarded by mutex_
  bool closing_ = false;         // guarded by mutex_
};

}  // namespace sayso::aec

#endif  // SAYSO_AEC_DIAGNOSTIC_RECORDER_H_
