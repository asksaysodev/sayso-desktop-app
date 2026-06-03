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

        apiClient.get('sales-coach/settings')
            .then((res) => {
                if (cancelled) return;
                const settings = res.data?.coachSettings;
                if (settings && typeof settings.open_last_used === 'boolean') {
                    setOpenLastUsed(settings.open_last_used);
                }
            })
            .catch(() => {
                // On error, keep default (true) — matches DB column default
            })
            .finally(() => {
                if (!cancelled) setLoaded(true);
            });

        return () => { cancelled = true; };
    }, []);

    return { openLastUsed, loaded };
}
