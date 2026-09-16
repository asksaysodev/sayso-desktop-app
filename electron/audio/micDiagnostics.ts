import { powerMonitor, screen } from 'electron';

import type { IAudioProvider, MicDeviceSnapshot, MicInputDiagnostics } from './IAudioProvider';

// SAYSO-431: context attached to the two mic events that only said "it failed" — the mid-session
// stall (SAYSO-APP-AE) and every mic start failure (SAYSO-APP-AC and siblings). Pure reporting: it
// never changes when or whether the mic restarts.

/** Which JS path asked native to start the mic. The same `no_tap_buffers` means something different
 *  on a user click than on the stall watchdog's eighth restart in a row. */
export type MicStartOrigin = 'user' | 'delivery_check' | 'watchdog';

/** HAL reads are IPC to coreaudiod — the daemon most likely to be wedged when these events fire.
 *  A cold snapshot of a built-in mic measured ~60ms; Bluetooth routes read slower. */
const MIC_DIAGNOSTICS_TIMEOUT_MS = 500;

const systemEventAtMs = { suspend: 0, resume: 0, displayAdded: 0, displayRemoved: 0 };
let listenersInstalled = false;

/** Call once after app `ready` — `screen` throws before it. */
export function installMicDiagnosticsListeners(): void {
  if (listenersInstalled) return;
  listenersInstalled = true;
  powerMonitor.on('suspend', () => { systemEventAtMs.suspend = Date.now(); });
  powerMonitor.on('resume', () => { systemEventAtMs.resume = Date.now(); });
  screen.on('display-added', () => { systemEventAtMs.displayAdded = Date.now(); });
  screen.on('display-removed', () => { systemEventAtMs.displayRemoved = Date.now(); });
}

export interface MicDiagnosticsReport {
  tags: Record<string, string>;
  /** One flat object per key: Sentry's default normalizeDepth (3) would truncate a nested snapshot. */
  extra: Record<string, unknown>;
  /** Single line for the packaged log file. */
  summary: string;
}

function secondsSince(atMs: number): number | null {
  return atMs > 0 ? Math.round((Date.now() - atMs) / 1000) : null;
}

function hogMode(device: MicDeviceSnapshot | null): string {
  if (!device || device.hogModePid === null) return 'unknown';
  if (device.hogModePid === -1) return 'none';
  return device.hogModePid === process.pid ? 'self' : 'other_process';
}

/** Buffers arrive every ~100ms, so a couple of seconds without one is a real gap, not jitter. */
const TAP_QUIET_MS = 2000;

/** The question a stall report has to answer: did the engine stop handing us audio (the OS side —
 *  route change, another app, a stopped graph), or is it still calling us while we drop it (our side)? */
function tapState(snapshot: MicInputDiagnostics): string {
  const { tap } = snapshot;
  if (tap.callbacks === 0) return 'no_callbacks';
  if (tap.msSinceLastCallback !== null && tap.msSinceLastCallback > TAP_QUIET_MS) return 'callbacks_stopped';
  if (tap.msSinceLastDelivered === null || tap.msSinceLastDelivered > TAP_QUIET_MS) return 'dropping';
  return 'delivering';
}

function routeState(snapshot: MicInputDiagnostics): string {
  if (snapshot.openedInputId === null) return 'not_open';
  return snapshot.openedInput ? 'stale_device' : 'default';
}

async function takeSnapshot(provider: IAudioProvider | null): Promise<{ status: string; snapshot: MicInputDiagnostics | null }> {
  if (!provider || typeof provider.getMicInputDiagnostics !== 'function') {
    return { status: 'unavailable', snapshot: null };
  }
  let timer: ReturnType<typeof setTimeout> | undefined;
  try {
    const timeout = new Promise<'timeout'>((resolve) => {
      timer = setTimeout(() => resolve('timeout'), MIC_DIAGNOSTICS_TIMEOUT_MS);
    });
    const result = await Promise.race([provider.getMicInputDiagnostics(), timeout]);
    if (result === 'timeout') return { status: 'timeout', snapshot: null };
    // null = native build without the method, or a previous snapshot still stuck in coreaudiod.
    return result ? { status: 'ok', snapshot: result } : { status: 'unavailable', snapshot: null };
  } catch {
    return { status: 'error', snapshot: null };
  } finally {
    clearTimeout(timer);
  }
}

/** Never throws, and resolves within MIC_DIAGNOSTICS_TIMEOUT_MS. */
export async function collectMicDiagnostics(
  provider: IAudioProvider | null,
  context: { origin: MicStartOrigin; sessionId?: string | null; msSinceLastUserChunk?: number | null },
): Promise<MicDiagnosticsReport> {
  const { status, snapshot } = await takeSnapshot(provider);

  const timing = {
    sessionId: context.sessionId ?? null,
    msSinceLastUserChunk: context.msSinceLastUserChunk ?? null,
    secondsSinceResume: secondsSince(systemEventAtMs.resume),
    secondsSinceSuspend: secondsSince(systemEventAtMs.suspend),
    secondsSinceDisplayAdded: secondsSince(systemEventAtMs.displayAdded),
    secondsSinceDisplayRemoved: secondsSince(systemEventAtMs.displayRemoved),
    msSinceDefaultInputChange: snapshot?.msSinceDefaultInputChange ?? null,
    msSinceEngineConfigChange: snapshot?.msSinceEngineConfigChange ?? null,
  };

  const tags: Record<string, string> = { mic_start_origin: context.origin, mic_diag: status };
  const extra: Record<string, unknown> = { mic_timing: timing };
  let summary = `origin=${context.origin} diag=${status}`;

  if (snapshot) {
    const input = snapshot.defaultInput;
    tags.mic_transport = input?.transport ?? 'none';
    tags.mic_hog_mode = hogMode(input);
    tags.mic_running_somewhere = input && input.isRunningSomewhere !== null ? String(input.isRunningSomewhere) : 'unknown';
    tags.mic_tap = tapState(snapshot);
    tags.mic_engine_running = String(snapshot.engineRunning);
    tags.mic_route = routeState(snapshot);

    const { defaultInput, openedInput, tap, ...engine } = snapshot;
    extra.mic_default_input = defaultInput;
    extra.mic_opened_input = openedInput;
    extra.mic_tap = tap;
    extra.mic_engine = engine;

    const drops = tap.droppedNoHandle + tap.droppedNoCallback + tap.droppedEmpty + tap.droppedLayout + tap.droppedUnsupported;
    summary +=
      ` input="${input?.name ?? 'none'}" transport=${tags.mic_transport} channels=${input?.inputChannels ?? '?'}` +
      ` running_somewhere=${tags.mic_running_somewhere} hog=${tags.mic_hog_mode} muted=${input?.inputMuted ?? '?'}` +
      ` engine_running=${snapshot.engineRunning} route=${tags.mic_route} recovering=${snapshot.routeRecovering}` +
      ` engine_builds=${snapshot.engineBuilds} tap=${tap.delivered}/${tap.callbacks} drops=${drops}` +
      ` tap_format=${tap.lastSampleRate ?? '?'}Hz/${tap.lastChannels ?? '?'}ch` +
      ` since_last_tap_ms=${tap.msSinceLastCallback ?? 'never'}` +
      ` since_config_change_ms=${snapshot.msSinceEngineConfigChange ?? 'never'}` +
      ` since_default_change_ms=${snapshot.msSinceDefaultInputChange ?? 'never'}`;
  }

  summary +=
    ` since_user_chunk_ms=${timing.msSinceLastUserChunk ?? 'n/a'} since_resume_s=${timing.secondsSinceResume ?? 'never'}` +
    ` since_display_change_s=${
      [timing.secondsSinceDisplayAdded, timing.secondsSinceDisplayRemoved]
        .filter((s): s is number => s !== null)
        .sort((a, b) => a - b)[0] ?? 'never'
    }`;

  return { tags, extra, summary };
}
