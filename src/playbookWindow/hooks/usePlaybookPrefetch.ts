import { useEffect } from 'react';
import * as Sentry from '@sentry/electron/renderer';
import { getPlaybooks } from '@/playbookWindow/services/playbookServices';
import { PlaybooksCachePayload } from '@/playbookWindow/types';

export function usePlaybookPrefetch() {
    useEffect(() => {
        let cancelled = false;

        const fetchAndCache = async () => {
            try {
                const playbooks = await getPlaybooks();
                if (cancelled) return;
                const payload: PlaybooksCachePayload = { playbooks, error: null };
                window.electron?.ipcRenderer?.send('set-playbooks-cache', payload);
            } catch (error) {
                if (cancelled) return;
                Sentry.captureException(error);
                const message = error instanceof Error ? error.message : 'Failed to load playbooks';
                const payload: PlaybooksCachePayload = { playbooks: null, error: message };
                window.electron?.ipcRenderer?.send('set-playbooks-cache', payload);
            }
        };

        fetchAndCache();

        return () => {
            cancelled = true;
        };
    }, []);
}
