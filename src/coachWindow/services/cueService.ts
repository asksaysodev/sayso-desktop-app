import * as Sentry from "@sentry/electron/renderer";
import { isTransientApiError, isAuthTeardownError } from '@/utils/errorReporting';

export const CUE_PERMISSIONS_DENIED = 'permissions_denied';

/**
 * Carries the `error` string returned by the main-process `start-cue` handler,
 * so callers can branch on the reason instead of parsing the message.
 */
export class CueStartError extends Error {
    readonly code?: string;

    constructor(code?: string) {
        super(code ? `Failed to start cue streaming: ${code}` : 'Failed to start cue streaming');
        this.name = 'CueStartError';
        this.code = code;
    }
}

/**
 * True when the start failed only because the OS permissions aren't granted.
 * Main already returns early and routes the user to the permissions UI, so this
 * is expected state — not an exception worth reporting.
 */
export const isCuePermissionsDeniedError = (error: unknown): boolean => (
    error instanceof CueStartError && error.code === CUE_PERMISSIONS_DENIED
);

/**
 * Single entry point for coach-window error reporting. Swallows the three classes
 * of failure that are expected state rather than bugs — OS permissions denied,
 * transient network loss, and post-logout auth teardown — and reports the rest.
 */
export const reportCoachError = (error: unknown): void => {
    if (
        isCuePermissionsDeniedError(error) ||
        isTransientApiError(error) ||
        isAuthTeardownError(error)
    ) {
        console.warn('[Coach] expected failure, not reported:', error);
        return;
    }
    Sentry.captureException(error);
};

export const cue_startStreaming = async (sessionId: string) => {
    try {
        if(!sessionId) {
            throw new Error('Session ID is required');
        }

        if (!window.electron?.cue) {
            throw new Error('Electron cue API not available');
        }

        const token: string | null = await window.electron?.ipcRenderer?.invoke('auth:get-token') ?? null;

        if (!token) {
            throw new Error('No session found');
        }

        const result = await window.electron.cue.start({
            sessionId,
            token
        });

        if(!result || !result.success) {
            throw new CueStartError(result?.error);
        }

        return result;

    } catch (error) {
        console.error('Error starting cue streaming:', error);
        throw error;
    }
}

export const cue_stopStreaming = async () => {
    try {
        if (!window.electron?.cue) {
            throw new Error('Electron cue API not available');
        }

        await window.electron.cue.stop();

    } catch (error) {
        console.error('Error stopping cue streaming:', error);
        throw error;
    }
}