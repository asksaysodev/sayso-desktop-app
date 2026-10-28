import type { MicSession } from './micActivityMonitor';

// The "Looks like you've started a call. Launch Sayso?" prompt (SAYSO-529).
// Pure state and rules; the window itself is created in main.ts, which passes
// its callbacks in through initCallPrompt().

// Mirrored by --auto-hide-duration (the Dismiss countdown fill) in
// src/callDetectedWindow/styles/CallDetected.css.
export const CALL_PROMPT_AUTO_HIDE_MS = 10000;
// Not shown by then (page failed to load): give up without marking the call dismissed.
export const CALL_PROMPT_LOAD_TIMEOUT_MS = 5000;
// Slide in / out. Mirrored by --enter-duration / --exit-duration in
// src/callDetectedWindow/styles/CallDetected.css.
export const CALL_PROMPT_ENTER_MS = 250;
export const CALL_PROMPT_EXIT_MS = 200;

export type CallPromptHideReason =
  | 'launch'
  | 'dismiss'
  | 'timeout'
  | 'call-ended'
  | 'coach-opened'
  | 'signed-out'
  | 'disabled'
  | 'load-failed';

export interface CallPromptDeps {
  canPrompt: () => boolean;
  isCoachOpen: () => boolean;
  isCueActive: () => boolean;
  openWindow: () => void;
  // animate: slide the card out before the window closes (timeout, dismiss)
  closeWindow: (animate: boolean) => void;
  launchCoach: () => void;
}

let deps: CallPromptDeps | null = null;
let activeSessionId: string | null = null;
let autoHideTimer: NodeJS.Timeout | null = null;
// Set from open until the window is shown.
let loadTimer: NodeJS.Timeout | null = null;
// Auto-hide pauses while the pointer is over the card, like a notification.
let autoHideRemainingMs = 0;
let autoHideStartedAt = 0;
// Sessions the user dismissed (or let time out): "not for this call". Cleared
// when the call ends, so the next call prompts again.
const dismissedSessionIds = new Set<string>();

export function initCallPrompt(callbacks: CallPromptDeps): void {
  deps = callbacks;
}

export function isCallPromptShowing(): boolean {
  return activeSessionId !== null;
}

/** Single entry point for showing the prompt. */
export function showCallDetectedPrompt(session: MicSession): void {
  if (!deps) return;
  if (activeSessionId !== null) return;
  if (dismissedSessionIds.has(session.id)) return;
  if (deps.isCoachOpen() || deps.isCueActive()) return;

  activeSessionId = session.id;
  deps.openWindow();
  autoHideRemainingMs = CALL_PROMPT_AUTO_HIDE_MS;
  loadTimer = setTimeout(() => hideCallDetectedPrompt('load-failed'), CALL_PROMPT_LOAD_TIMEOUT_MS);
  console.log(`[CallDetection] prompt shown (session ${session.id})`);
}

/**
 * Main calls this once the window is on screen. The countdown fill starts when
 * the page renders, so the auto-hide starts here too, not at open.
 */
export function onCallPromptShown(): void {
  if (activeSessionId === null || !loadTimer) return;
  clearTimeout(loadTimer);
  loadTimer = null;
  startAutoHide();
}

function startAutoHide(): void {
  autoHideStartedAt = Date.now();
  autoHideTimer = setTimeout(() => hideCallDetectedPrompt('timeout'), autoHideRemainingMs);
}

/** Pointer over the card pauses the auto-hide; leaving resumes the remainder. */
export function setCallPromptHovered(hovered: boolean): void {
  if (activeSessionId === null || loadTimer) return;
  if (hovered && autoHideTimer) {
    clearTimeout(autoHideTimer);
    autoHideTimer = null;
    autoHideRemainingMs = Math.max(0, autoHideRemainingMs - (Date.now() - autoHideStartedAt));
  } else if (!hovered && !autoHideTimer) {
    startAutoHide();
  }
}

export function hideCallDetectedPrompt(reason: CallPromptHideReason): void {
  if (autoHideTimer) {
    clearTimeout(autoHideTimer);
    autoHideTimer = null;
  }
  if (loadTimer) {
    clearTimeout(loadTimer);
    loadTimer = null;
  }
  // The monitor stops on sign-out without ending its sessions.
  if (reason === 'signed-out') dismissedSessionIds.clear();
  if (activeSessionId === null) return;

  if (reason === 'dismiss' || reason === 'timeout') dismissedSessionIds.add(activeSessionId);
  console.log(`[CallDetection] prompt hidden: ${reason} (session ${activeSessionId})`);
  activeSessionId = null;
  // Both mean "not for this call" and leave the same way; the rest are
  // instant because something else takes over (coach) or the call is gone.
  deps?.closeWindow(reason === 'timeout' || reason === 'dismiss');
}

export function onCallCandidate(session: MicSession): void {
  if (!deps?.canPrompt()) return;
  showCallDetectedPrompt(session);
}

export function onCallEnded(session: MicSession): void {
  dismissedSessionIds.delete(session.id);
  if (activeSessionId === session.id) hideCallDetectedPrompt('call-ended');
}

/** The prompt's CTA. Change this to start a session once the CTA does that. */
export function launchFromCallPrompt(): void {
  if (activeSessionId === null) return;
  hideCallDetectedPrompt('launch');
  deps?.launchCoach();
}
