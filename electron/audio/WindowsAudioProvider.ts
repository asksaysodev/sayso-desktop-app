import type { IAudioProvider } from './IAudioProvider';

// Delegates to the existing Windows native module singleton (WASAPI backend,
// src/audio_device_manager_win.cpp). native-audio/index.js is a single,
// platform-agnostic JS wrapper shared by both macOS and Windows builds — it
// always loads build/Release/native_audio.node, and binding.gyp's
// conditions block is what compiles that file from the Windows source on
// this platform. Mirrors MacAudioProvider.ts exactly; native-audio is
// excluded from tsconfig compilation (JS/C++ only).
const provider: IAudioProvider = require('../native-audio');

export default provider;
