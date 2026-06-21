"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const platformModules = {
    darwin: './MacAudioProvider',
    win32: './WindowsAudioProvider',
};
const modulePath = platformModules[process.platform];
if (!modulePath) {
    throw new Error(`[Audio] Unsupported platform: ${process.platform}`);
}
const provider = require(modulePath).default;
exports.default = provider;
//# sourceMappingURL=index.js.map