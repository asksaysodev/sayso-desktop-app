import type { IAudioProvider } from './IAudioProvider';

// Delegates to the existing macOS native module singleton.
// native-audio is excluded from tsconfig compilation (JS/C++ only).
const provider: IAudioProvider = require('../native-audio');

export default provider;
