import { app, systemPreferences, shell } from 'electron';
import fs from 'node:fs';
import path from 'node:path';

import * as audioManager from '../audio/audioManager';
import type { IPermissionsProvider, PermissionsStatus, RequestMicResult } from './IPermissionsProvider';

function completeFlagPath(): string {
  return path.join(app.getPath('userData'), 'permissions-complete');
}

class MacPermissionsProvider implements IPermissionsProvider {
  async checkGranted(): Promise<PermissionsStatus> {
    const mic = systemPreferences.getMediaAccessStatus('microphone') === 'granted';
    // Use CGPreflightScreenCaptureAccess (non-prompting) to READ status — never triggers the macOS
    // dialog. The dialog is only shown on explicit user action via requestScreen().
    const screen = audioManager.isScreenRecordingGranted({ warnIfUnavailable: true });
    return { granted: mic && screen, mic, screen };
  }

  isComplete(): boolean {
    const mic = systemPreferences.getMediaAccessStatus('microphone') === 'granted';
    // CGPreflight is accurate for the running process (screen-recording grant is bound at launch).
    const screen = audioManager.isScreenRecordingGranted();
    // Live OS state is authoritative: if both are actually granted for this process, onboarding is
    // complete regardless of the flag. This self-heals the case where the flag is missing but perms
    // work — e.g. after the cert migration deletes the flag and the user re-grants + reopens. We only
    // short-circuit on live grants, so an optimistically-written flag without a real SCK grant
    // (screen === false) still routes back to /permissions.
    if (mic && screen) {
      if (!fs.existsSync(completeFlagPath())) {
        try {
          fs.writeFileSync(completeFlagPath(), '1');
        } catch (e) {
          console.warn('[Permissions] Failed to self-heal permissions-complete flag:', e);
        }
      }
      return true;
    }
    return false;
  }

  async requestMic(): Promise<RequestMicResult> {
    const micStatus = systemPreferences.getMediaAccessStatus('microphone');
    if (micStatus === 'granted') return { mic: true, action: 'already-granted' };
    if (micStatus === 'not-determined') {
      const granted = await systemPreferences.askForMediaAccess('microphone');
      return { mic: granted, action: 'asked' };
    }
    // denied / restricted → open Microphone privacy pane directly
    await shell.openExternal('x-apple.systempreferences:com.apple.preference.security?Privacy_Microphone');
    return { mic: false, action: 'open-settings' };
  }

  requestScreen(): void {
    // Screen Recording is required by ScreenCaptureKit; the prompt lives in the native audio module.
    audioManager.requestScreenRecordingPermission();
  }

  async openScreenSettings(): Promise<void> {
    await shell.openExternal('x-apple.systempreferences:com.apple.preference.security?Privacy_ScreenCapture');
  }

  markComplete(): void {
    fs.writeFileSync(completeFlagPath(), '1');
  }
}

export default new MacPermissionsProvider();
