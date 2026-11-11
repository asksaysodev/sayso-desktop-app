import { app, systemPreferences, shell } from 'electron';
import fs from 'node:fs';
import path from 'node:path';

import * as audioManager from '../audio/audioManager';
import type {
  IPermissionsProvider,
  PermissionRequirements,
  PermissionsStatus,
  RequestMicResult,
  SystemAudioPermissionState,
} from './IPermissionsProvider';

function completeFlagPath(): string {
  return path.join(app.getPath('userData'), 'permissions-complete');
}

interface SystemAudioGate {
  satisfied: boolean;
  state: SystemAudioPermissionState | null;
}

class MacPermissionsProvider implements IPermissionsProvider {
  get requirements(): PermissionRequirements {
    return audioManager.usesCoreAudioTap()
      ? { screen: true, relaunchOnComplete: false, systemAudio: 'audio-capture' }
      : { screen: true, relaunchOnComplete: true, systemAudio: 'screen-recording' };
  }

  private readSystemAudio(opts?: { warnIfUnavailable?: boolean }): SystemAudioGate {
    if (audioManager.usesCoreAudioTap()) {
      const state = audioManager.getSystemAudioPermission();
      return { satisfied: state === 'granted' || state === 'unknown' || state === null, state };
    }
    // Use CGPreflightScreenCaptureAccess (non-prompting) to READ status — never triggers the macOS
    // dialog. The dialog is only shown on explicit user action via requestScreen().
    return { satisfied: audioManager.isScreenRecordingGranted(opts), state: null };
  }

  async checkGranted(): Promise<PermissionsStatus> {
    const mic = systemPreferences.getMediaAccessStatus('microphone') === 'granted';
    const gate = this.readSystemAudio({ warnIfUnavailable: true });
    return { granted: mic && gate.satisfied, mic, screen: gate.satisfied, systemAudio: gate.state };
  }

  async checkMic(): Promise<boolean> {
    return systemPreferences.getMediaAccessStatus('microphone') === 'granted';
  }

  isComplete(): boolean {
    const mic = systemPreferences.getMediaAccessStatus('microphone') === 'granted';
    const screen = this.readSystemAudio().satisfied;
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

  async requestScreen(): Promise<SystemAudioPermissionState | null> {
    if (audioManager.usesCoreAudioTap()) {
      return audioManager.requestSystemAudioPermission();
    }
    audioManager.requestScreenRecordingPermission();
    return null;
  }

  async openScreenSettings(): Promise<void> {
    await shell.openExternal('x-apple.systempreferences:com.apple.preference.security?Privacy_ScreenCapture');
  }

  markComplete(): void {
    fs.writeFileSync(completeFlagPath(), '1');
    console.log('[Permissions] permissions-complete flag written');
  }
}

export default new MacPermissionsProvider();
