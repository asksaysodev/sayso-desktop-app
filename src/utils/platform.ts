// Renderer-side mirror of electron/utils/platform.ts, which can't be imported here.
// The userAgent fallback only applies outside Electron, where no preload has run.
function resolvePlatform(): string {
    const fromBridge = window.sayso?.platform;
    if (fromBridge) return fromBridge;
    if (navigator.userAgent.includes('Windows')) return 'win32';
    if (navigator.userAgent.includes('Mac')) return 'darwin';
    return 'linux';
}

const platform = resolvePlatform();
document.documentElement.dataset.platform = platform;

export const IS_MAC = platform === 'darwin';
export const IS_WINDOWS = platform === 'win32';
