import type { IPermissionsProvider } from './IPermissionsProvider';

const platformModules: Partial<Record<NodeJS.Platform, string>> = {
  darwin: './MacPermissionsProvider',
  win32: './WindowsPermissionsProvider',
};

// Unlike audio/index.ts (whose throw is swallowed by initAudioProvider's
// try/catch), this module is imported statically by permissionsManager, which is
// imported by main.ts BEFORE Sentry.init() — so a throw here is a silent,
// unreported startup crash. Degrade gracefully instead: fall back to the Windows
// safe-default provider (granted/complete = true) so an unsupported platform
// never blocks on a permission step that doesn't exist. Only darwin + win32 are
// real targets (see docs/IPC_CONTRACT.md).
const modulePath = platformModules[process.platform];
if (!modulePath) {
  console.warn(
    `[Permissions] Unsupported platform '${process.platform}' — using safe-default (Windows) permissions provider.`,
  );
}

const provider: IPermissionsProvider = require(modulePath ?? platformModules.win32!).default;

export default provider;
