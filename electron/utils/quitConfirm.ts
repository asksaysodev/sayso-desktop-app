import { app, dialog, type MessageBoxOptions } from 'electron';
import { IS_MAC } from './platform';

// Quit confirmation (SAYSO-613). Only the user's own quit entry points come
// through here: the tray's Quit item and ⌘Q. Every other quit (update install,
// signals, OS shutdown/logout) calls app.quit() directly and never prompts,
// which is why this isn't a before-quit hook: a dialog there would hold up a
// macOS shutdown.

export interface QuitConfirmDeps {
  isCueActive: () => boolean;
  isCallDetectionActive: () => boolean;
  isQuitting: () => boolean;
}

let deps: QuitConfirmDeps | null = null;
// One dialog at a time: a second click or ⌘Q while it is open does nothing.
let dialogOpen = false;

export function initQuitConfirm(callbacks: QuitConfirmDeps): void {
  deps = callbacks;
}

// One state shows, in priority order: Cue session → call detection → default.
function dialogOptions(): MessageBoxOptions {
  const base = { cancelId: 1, defaultId: 0, noLink: true };

  if (deps?.isCueActive()) {
    return {
      ...base,
      type: 'warning',
      message: 'End your coaching session?',
      detail: 'Sayso is coaching a live call. Quitting will end the session.',
      buttons: ['End Session & Quit', 'Cancel'],
    };
  }

  const detail = deps?.isCallDetectionActive()
    ? "Sayso won't detect your meetings or offer to launch the coach until you open it again."
    : `You can reopen it anytime from ${IS_MAC ? 'Applications' : 'the Start menu'}.`;

  return { ...base, type: 'none', message: 'Quit Sayso?', detail, buttons: ['Quit', 'Cancel'] };
}

/** Ask, then quit if the user confirms. Cue keeps running while the dialog is open. */
export async function requestQuit(): Promise<void> {
  if (dialogOpen || deps?.isQuitting()) return;
  dialogOpen = true;
  try {
    // The tray popover hides on blur, so the dialog has no parent; bring the
    // app forward or it can open behind whatever app the user was in.
    if (IS_MAC) app.focus({ steal: true });
    const { response } = await dialog.showMessageBox(dialogOptions());
    if (response === 0) app.quit();
  } catch (err) {
    // Never leave the user unable to quit.
    console.warn('[Quit] confirmation dialog failed, quitting without it', err);
    app.quit();
  } finally {
    dialogOpen = false;
  }
}
