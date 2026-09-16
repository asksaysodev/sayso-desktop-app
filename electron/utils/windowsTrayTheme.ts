// Windows paints the taskbar from its own theme setting, separate from the one
// Chromium reports via `nativeTheme.shouldUseDarkColors` (that mirrors
// AppsUseLightTheme — the in-app theme). A user can run light apps on a dark
// taskbar and vice versa, so the tray glyph has to read SystemUsesLightTheme
// directly. There is no Electron API for it; `reg query` is the cheap way.
import { execFile } from 'child_process';

const PERSONALIZE_KEY =
  'HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize';
const VALUE = 'SystemUsesLightTheme';

/**
 * Resolves true when the Windows taskbar is painted light (so a tray glyph
 * needs to be dark). Defaults to false — the Windows 11 default is a dark
 * taskbar, and that is also the safer guess if the value is missing or the
 * query fails.
 */
export function isWindowsTaskbarLight(): Promise<boolean> {
  return new Promise((resolve) => {
    execFile(
      'reg',
      ['query', PERSONALIZE_KEY, '/v', VALUE],
      { windowsHide: true, timeout: 3000 },
      (error, stdout) => {
        if (error) {
          resolve(false);
          return;
        }
        const match = new RegExp(`${VALUE}\\s+REG_DWORD\\s+0x([0-9a-fA-F]+)`).exec(stdout);
        resolve(match ? parseInt(match[1], 16) === 1 : false);
      }
    );
  });
}
