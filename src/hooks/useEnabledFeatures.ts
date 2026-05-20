import { useCallback, useEffect, useState } from 'react';

export function useEnabledFeatures() {
    const [enabledFeatures, setEnabledFeatures] = useState<string[]>([]);

    useEffect(() => {
        const ipcRenderer = window.electron?.ipcRenderer;
        if (!ipcRenderer) return;

        const handleFeaturesChanged = (data: unknown) => {
            setEnabledFeatures((data as { enabledFeatures: string[] }).enabledFeatures ?? []);
        };

        ipcRenderer.on('enabled-features-changed', handleFeaturesChanged);
        ipcRenderer.send('get-enabled-features');

        return () => {
            ipcRenderer.off('enabled-features-changed', handleFeaturesChanged);
        };
    }, []);

    const hasFeature = useCallback((key: string) => enabledFeatures.includes(key), [enabledFeatures]);

    return { enabledFeatures, hasFeature };
}
