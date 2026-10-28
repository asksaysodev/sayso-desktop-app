import { EventEmitter } from 'node:events';
import { randomUUID } from 'node:crypto';
import { app } from 'electron';
import { isBrowserApp } from './browserAllowlist';

/** How long a browser must hold the mic before it counts as a call (SAYSO-528). */
export const CALL_DETECTION_DEBOUNCE_MS = 3000;

/**
 * How long a session survives with no pid on the mic. Browsers briefly close and
 * reopen capture (Safari: stop→start ~50ms apart when joining Meet); without
 * this, that flicker would end the call and start a "new" one, re-prompting
 * after a dismiss.
 */
export const MIC_STOP_GRACE_MS = 1500;

/** `unsupported`: macOS < 14.2, where only a device-wide flag exists (see docs/CALL_DETECTION.md). */
type MicActivityMode = 'per-process' | 'unsupported';

interface MicActivityEvent {
  type: 'start' | 'stop';
  pid: number;
  responsiblePid: number;
  bundleId?: string;
  appPath?: string;
  appBundleId?: string;
  exePath?: string;
  ts: number;
}

interface MicActivityAddon {
  start(callback: (event: MicActivityEvent) => void): void;
  stop(): void;
  mode(): MicActivityMode;
}

/** One app's continuous use of the mic. */
export interface MicSession {
  id: string;
  appKey: string;
  appBundleId?: string;
  exePath?: string;
  startedAt: number;
}

interface TrackedSession extends MicSession {
  pids: Set<number>;
  candidate: boolean;
  debounceTimer: NodeJS.Timeout | null;
  graceTimer: NodeJS.Timeout | null;
}

/**
 * `call-candidate` (MicSession): a browser has held the mic for
 * CALL_DETECTION_DEBOUNCE_MS.
 * `call-ended` (MicSession): that session released the mic.
 */
export const callDetection = new EventEmitter();

let addon: MicActivityAddon | null = null;
let addonLoadFailed = false;
let running = false;
const sessions = new Map<string, TrackedSession>();
/**
 * Stops are matched by pid, not by appKey: on macOS a stop for a pid that already
 * exited can't be resolved back to its app (it comes back as the helper's own
 * bundle id), and looking it up by appKey would leak the session for good.
 */
const sessionByPid = new Map<number, TrackedSession>();

function loadAddon(): MicActivityAddon | null {
  if (addon || addonLoadFailed) return addon;
  try {
    addon = require('../mic-activity') as MicActivityAddon;
  } catch (err) {
    // A missing .node build must never take the app down — detection is optional.
    addonLoadFailed = true;
    console.warn('[CallDetection] mic-activity addon unavailable, detection disabled', err);
  }
  return addon;
}

function appKeyOf(event: MicActivityEvent): string {
  return event.appBundleId || event.appPath || event.exePath || event.bundleId || `pid:${event.pid}`;
}

/** Sayso's own capture: Cue runs in main, the Settings mic test in a renderer helper. */
function isSayso(event: MicActivityEvent): boolean {
  const ownPids = new Set([process.pid, ...app.getAppMetrics().map((metric) => metric.pid)]);
  if (ownPids.has(event.pid) || ownPids.has(event.responsiblePid)) return true;
  if (event.appPath && process.execPath.startsWith(`${event.appPath}/`)) return true;
  if (event.exePath && event.exePath.toLowerCase() === process.execPath.toLowerCase()) return true;
  return false;
}

function publicSession(session: TrackedSession): MicSession {
  const { id, appKey, appBundleId, exePath, startedAt } = session;
  return { id, appKey, appBundleId, exePath, startedAt };
}

function emitCandidate(session: TrackedSession): void {
  session.candidate = true;
  callDetection.emit('call-candidate', publicSession(session));
}

function endSession(session: TrackedSession): void {
  if (session.debounceTimer) clearTimeout(session.debounceTimer);
  if (session.graceTimer) clearTimeout(session.graceTimer);
  sessions.delete(session.appKey);
  for (const pid of session.pids) sessionByPid.delete(pid);
  console.log(`[CallDetection] mic stop: ${session.appKey} after ${Date.now() - session.startedAt}ms`);
  if (session.candidate) callDetection.emit('call-ended', publicSession(session));
}

function handleStart(event: MicActivityEvent): void {
  if (isSayso(event)) return;
  // A start with no matching stop: detach the pid from its old session first.
  if (sessionByPid.has(event.pid)) handleStop(event);

  const appKey = appKeyOf(event);
  const existing = sessions.get(appKey);
  if (existing) {
    existing.pids.add(event.pid);
    sessionByPid.set(event.pid, existing);
    if (existing.graceTimer) {
      // Back within the grace period: same call.
      clearTimeout(existing.graceTimer);
      existing.graceTimer = null;
      // The debounce elapsed while the mic was briefly off; it was held long enough.
      if (isBrowserApp(existing) && !existing.candidate && !existing.debounceTimer) emitCandidate(existing);
    }
    return;
  }

  const watched = isBrowserApp(event);
  console.log(`[CallDetection] mic start: ${appKey} pid=${event.pid} watched=${watched}`);
  const session: TrackedSession = {
    id: randomUUID(),
    appKey,
    appBundleId: event.appBundleId,
    exePath: event.exePath,
    startedAt: event.ts,
    pids: new Set([event.pid]),
    candidate: false,
    debounceTimer: null,
    graceTimer: null,
  };
  if (watched) {
    session.debounceTimer = setTimeout(() => {
      session.debounceTimer = null;
      // Mic off right now (inside the grace period): decide when it comes back.
      if (session.pids.size > 0) emitCandidate(session);
    }, CALL_DETECTION_DEBOUNCE_MS);
  }
  sessions.set(appKey, session);
  sessionByPid.set(event.pid, session);
}

function handleStop(event: MicActivityEvent): void {
  const session = sessionByPid.get(event.pid);
  if (!session) return;
  sessionByPid.delete(event.pid);
  session.pids.delete(event.pid);
  if (session.pids.size > 0 || session.graceTimer) return;

  session.graceTimer = setTimeout(() => endSession(session), MIC_STOP_GRACE_MS);
}

/** Start watching mic usage. Idempotent; a no-op when the addon isn't available. */
export function startMicActivityMonitor(): void {
  if (running) return;
  const micActivity = loadAddon();
  if (!micActivity) return;
  const mode = micActivity.mode();
  if (mode === 'unsupported') {
    console.log('[CallDetection] unsupported on this OS version (needs macOS 14.2+), detection disabled');
    return;
  }
  try {
    micActivity.start((event) => (event.type === 'start' ? handleStart(event) : handleStop(event)));
    running = true;
    console.log(`[CallDetection] monitor started (${mode})`);
  } catch (err) {
    console.warn('[CallDetection] monitor failed to start', err);
  }
}

/** Stop watching and forget all sessions. Ends in-progress candidates silently. */
export function stopMicActivityMonitor(): void {
  if (!running || !addon) return;
  addon.stop();
  running = false;
  for (const session of sessions.values()) {
    if (session.debounceTimer) clearTimeout(session.debounceTimer);
    if (session.graceTimer) clearTimeout(session.graceTimer);
  }
  sessions.clear();
  sessionByPid.clear();
  console.log('[CallDetection] monitor stopped');
}
