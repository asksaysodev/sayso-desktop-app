import { CueStartError } from '@/utils/errorReporting';

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