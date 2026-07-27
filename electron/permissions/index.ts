import type { IPermissionsProvider } from './IPermissionsProvider';

const platformModules: Partial<Record<NodeJS.Platform, string>> = {
  darwin: './MacPermissionsProvider',
  win32: './WindowsPermissionsProvider',
};

const modulePath = platformModules[process.platform];
if (!modulePath) {
  throw new Error(`[Permissions] Unsupported platform: ${process.platform}`);
}

const provider: IPermissionsProvider = require(modulePath).default;

export default provider;
