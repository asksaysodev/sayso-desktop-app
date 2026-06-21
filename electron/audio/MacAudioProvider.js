"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
// Delegates to the existing macOS native module singleton.
// native-audio is excluded from tsconfig compilation (JS/C++ only).
const provider = require('../native-audio');
exports.default = provider;
//# sourceMappingURL=MacAudioProvider.js.map