import type { MicSession } from './micActivityMonitor';

// The "Sayso identified a call" prompt (SAYSO-529, SAYSO-566). Pure state and
// rules; the window is the coach window in its prompt state, created in main.ts,
// which passes its callbacks in through initCallPrompt().

// Mirrored by --auto-hide-duration (the close button's countdown ring) in
// src/coachWindow/styles/CoachPrompt.css.
export const CALL_PROMPT_AUTO_HIDE_MS = 10000;
// Not shown by then (page failed to load): give up without marking the call dismissed.
export const CALL_PROMPT_LOAD_TIMEOUT_MS = 5000;
// Enter / exit. Mirrored by --enter-duration / --exit-duration in
// src/coachWindow/styles/CoachPrompt.css.
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
  | 'load-failed'
  | 'no-mic';

export interface CallPromptDeps {
  canPrompt: () => boolean;
  isCoachOpen: () => boolean;
  isCueActive: () => boolean;
  openWindow: () => void;
  // animate: play the exit before the window closes (timeout, dismiss)
  closeWindow: (animate: boolean) => void;
  // Keep the window and turn it into the regular coach (launch, coach-opened)
  promoteWindow: () => void;
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
  // The window becomes the coach: a lead was picked, or the coach was opened
  // from elsewhere while the prompt was up.
  if (reason === 'launch' || reason === 'coach-opened') {
    deps?.promoteWindow();
    return;
  }
  // Both mean "not for this call" and leave the same way; the rest are
  // instant because the call is gone or the prompt can't go anywhere.
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

/**
 * A lead pill was picked: the window becomes the coach, whose renderer then
 * starts the session. False if no prompt is up (it timed out meanwhile).
 */
export function launchFromCallPrompt(): boolean {
  if (activeSessionId === null) return false;
  hideCallDetectedPrompt('launch');
  return true;
}
