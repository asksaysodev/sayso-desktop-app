import type { IAudioProvider } from './IAudioProvider';

const platformModules: Partial<Record<NodeJS.Platform, string>> = {
  darwin: './MacAudioProvider',
  win32: './WindowsAudioProvider',
};

const modulePath = platformModules[process.platform];
if (!modulePath) {
  throw new Error(`[Audio] Unsupported platform: ${process.platform}`);
}

const provider: IAudioProvider = require(modulePath).default;

export default provider;
