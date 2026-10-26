import path from 'node:path';

// v1 watches browsers only (most calls run in a CRM dialer tab). Matched against
// the *owning app* — Chrome's mic user is a com.google.Chrome.helper process, but
// the monitor resolves it to Google Chrome.app — so helper ids don't belong here.
const MAC_BROWSER_BUNDLE_IDS = new Set([
  'com.google.Chrome',
  'com.google.Chrome.beta',
  'com.google.Chrome.dev',
  'com.google.Chrome.canary',
  'com.microsoft.edgemac',
  'com.microsoft.edgemac.Beta',
  'com.microsoft.edgemac.Dev',
  'com.microsoft.edgemac.Canary',
  'company.thebrowser.Browser', // Arc
  'com.brave.Browser',
  'com.apple.Safari',
  'com.apple.SafariTechnologyPreview',
  'org.mozilla.firefox',
  'org.mozilla.firefoxdeveloperedition',
  'com.operasoftware.Opera',
  'com.vivaldi.Vivaldi',
]);

const WINDOWS_BROWSER_EXES = new Set([
  'chrome.exe',
  'msedge.exe',
  'brave.exe',
  'firefox.exe',
  'arc.exe',
  'opera.exe',
  'vivaldi.exe',
]);

export function isBrowserApp(app: { appBundleId?: string; exePath?: string }): boolean {
  if (app.appBundleId) return MAC_BROWSER_BUNDLE_IDS.has(app.appBundleId);
  if (app.exePath) return WINDOWS_BROWSER_EXES.has(path.win32.basename(app.exePath).toLowerCase());
  return false;
}
