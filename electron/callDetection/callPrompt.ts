import type { MicSession } from './micActivityMonitor';

// The "Looks like you've started a call. Launch Sayso?" prompt (SAYSO-529).
// Pure state and rules; the window itself is created in main.ts, which passes
// its callbacks in through initCallPrompt().

export const CALL_PROMPT_AUTO_HIDE_MS = 15000;

export type CallPromptHideReason =
  | 'launch'
  | 'dismiss'
  | 'timeout'
  | 'call-ended'
  | 'coach-opened'
  | 'signed-out'
  | 'disabled';

export interface CallPromptDeps {
  canPrompt: () => boolean;
  isCoachOpen: () => boolean;
  isCueActive: () => boolean;
  openWindow: () => void;
  closeWindow: () => void;
  launchCoach: () => void;
}

let deps: CallPromptDeps | null = null;
let activeSessionId: string | null = null;
let autoHideTimer: NodeJS.Timeout | null = null;
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
  autoHideTimer = setTimeout(() => hideCallDetectedPrompt('timeout'), CALL_PROMPT_AUTO_HIDE_MS);
  console.log(`[CallDetection] prompt shown (session ${session.id})`);
}

export function hideCallDetectedPrompt(reason: CallPromptHideReason): void {
  if (autoHideTimer) {
    clearTimeout(autoHideTimer);
    autoHideTimer = null;
  }
  // The monitor stops on sign-out without ending its sessions.
  if (reason === 'signed-out') dismissedSessionIds.clear();
  if (activeSessionId === null) return;

  if (reason === 'dismiss' || reason === 'timeout') dismissedSessionIds.add(activeSessionId);
  console.log(`[CallDetection] prompt hidden: ${reason} (session ${activeSessionId})`);
  activeSessionId = null;
  deps?.closeWindow();
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
