import { useEffect, useState } from 'react';
import { Playbook, PlaybooksCachePayload } from '@/playbookWindow/types';

interface UsePlaybooksCacheResult {
    playbooks: Playbook[] | null;
    error: string | null;
    isLoading: boolean;
}

export function usePlaybooksCache(): UsePlaybooksCacheResult {
    const [playbooks, setPlaybooks] = useState<Playbook[] | null>(null);
    const [error, setError] = useState<string | null>(null);

    useEffect(() => {
        const ipcRenderer = window.electron?.ipcRenderer;
        if (!ipcRenderer) return;

        let cancelled = false;

        ipcRenderer.invoke('get-playbooks-cache').then((payload) => {
            if (cancelled) return;
            const cache = payload as PlaybooksCachePayload;
            setPlaybooks(cache.playbooks);
            setError(cache.error);
        });

        const handleUpdate = (data: unknown) => {
            const cache = data as PlaybooksCachePayload;
            setPlaybooks(cache.playbooks);
            setError(cache.error);
        };

        const offPlaybooksUpdated = ipcRenderer.on('playbooks-updated', handleUpdate);

        return () => {
            cancelled = true;
            offPlaybooksUpdated?.();
        };
    }, []);

    const isLoading = playbooks === null && error === null;

    return { playbooks, error, isLoading };
}
