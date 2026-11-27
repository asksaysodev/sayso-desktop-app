import type { CoachOpener } from '../shared/cueLaunchSource';

// How the open coach window was opened (SAYSO-696). Main records it when a coach window appears
// (created, or the call prompt promoted in place) and forgets it when the window closes; the
// renderer reads it with `coach:get-opener` when it starts a session. Focusing a coach that is
// already open doesn't call noteCoachOpened, so it keeps its opener.
let opener: CoachOpener | null = null;

export function noteCoachOpened(by: CoachOpener): void {
  opener = by;
}

export function noteCoachClosed(): void {
  opener = null;
}

// 'other' when nothing was recorded: an opener nobody wired up still sends a valid value.
export function getCoachOpener(): CoachOpener {
  return opener ?? 'other';
}
