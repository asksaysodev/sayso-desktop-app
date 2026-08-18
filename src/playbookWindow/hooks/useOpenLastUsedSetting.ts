import { useEffect, useState } from 'react';
import apiClient from '@/config/axios';

interface OpenLastUsedResult {
    openLastUsed: boolean;
    loaded: boolean;
}

export function useOpenLastUsedSetting(): OpenLastUsedResult {
    const [openLastUsed, setOpenLastUsed] = useState(true);
    const [loaded, setLoaded] = useState(false);

    useEffect(() => {
        let cancelled = false;
        // Network is authoritative — once it resolves, a late-arriving cache
        // response must not clobber it (they're independent async calls with
        // no guaranteed ordering; main could be briefly busy with a disk
        // write and answer the cache invoke after the HTTP round-trip lands).
        let networkResolved = false;
        const ipcRenderer = window.electron?.ipcRenderer;

        // Cache-first: unblocks the playbook window's auto-select immediately
        // instead of waiting on the network call below (SAYSO-367 follow-up).
        ipcRenderer?.invoke('get-open-last-used-cache').then((cached) => {
            if (cancelled || networkResolved) return;
            if (typeof cached === 'boolean') setOpenLastUsed(cached);
            setLoaded(true);
        });

        const offUpdate = ipcRenderer?.on('open-last-used-updated', (value: unknown) => {
            if (typeof value === 'boolean') setOpenLastUsed(value);
        });

        // Always revalidate over the network — the cache above may be stale
        // (e.g. toggled in Settings during a different session) — and push
        // the confirmed value back to main so the cache self-heals for the
        // next window open.
        apiClient.get('sales-coach/settings')
            .then((res) => {
                if (cancelled) return;
                networkResolved = true;
                const settings = res.data?.coachSettings;
                if (settings && typeof settings.open_last_used === 'boolean') {
                    setOpenLastUsed(settings.open_last_used);
                    ipcRenderer?.send('set-open-last-used-cache', settings.open_last_used);
                }
            })
            .catch(() => {
                // On error, keep whatever the cache (or the default true) gave us.
            })
            .finally(() => {
                if (!cancelled) setLoaded(true);
            });

        return () => {
            cancelled = true;
            offUpdate?.();
        };
    }, []);

    return { openLastUsed, loaded };
}
