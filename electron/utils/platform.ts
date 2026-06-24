// Centralized platform flags. `process.platform` is a static string, so this is
// about readability/consistency (and avoiding `'win32'` typos), not performance.
// Import these instead of re-checking process.platform at each call site.
export const IS_MAC = process.platform === 'darwin';
export const IS_WINDOWS = process.platform === 'win32';
export const IS_LINUX = process.platform === 'linux';
