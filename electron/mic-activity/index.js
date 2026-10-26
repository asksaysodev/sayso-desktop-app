// Loader for the mic-activity addon. Hand-written CommonJS (no .ts source),
// like electron/native-audio/index.js. Throws when the .node build is missing;
// callers require this lazily and treat that as "detection unavailable".
const path = require('path');

const modulePath = path.join(__dirname, 'build', 'Release', 'mic_activity.node');

/**
 * @typedef {Object} MicActivityEvent
 * @property {'start' | 'stop'} type
 * @property {number} pid
 * @property {number} responsiblePid  owning app's pid
 * @property {string} [bundleId]      macOS: bundle id Core Audio reports
 * @property {string} [appPath]       macOS: outermost .app of the responsible process
 * @property {string} [appBundleId]   macOS: bundle id of appPath
 * @property {string} [exePath]       Windows: process image path
 * @property {number} ts              ms since epoch
 */

/**
 * @type {{
 *   start(callback: (event: MicActivityEvent) => void): void,
 *   stop(): void,
 *   mode(): 'per-process' | 'unsupported',  // unsupported: macOS < 14.2, start() throws
 * }}
 */
module.exports = require(modulePath);
