import { useEffect, useState } from 'react';

export function useCoachSettingsWindow() {
    const [isCoachSettingsWindowOpen, setIsCoachSettingsWindowOpen] = useState(false);

    useEffect(() => {
        const ipcRenderer = window.electron?.ipcRenderer;
        if (!ipcRenderer) return;

        const handleCoachSettingsWindowState = (state: unknown) => {
            setIsCoachSettingsWindowOpen((state as { isOpen: boolean }).isOpen);
        };

        ipcRenderer.on('coach-settings-window-state', handleCoachSettingsWindowState);
        ipcRenderer.send('get-coach-settings-window-state');

        return () => {
            ipcRenderer.off('coach-settings-window-state', handleCoachSettingsWindowState);
        };
    }, []);

    const toggleCoachSettingsWindow = () => {
        const ipcRenderer = window.electron?.ipcRenderer;
        if (!ipcRenderer) return;

        if (isCoachSettingsWindowOpen) {
            ipcRenderer.send('close-coach-settings-window');
            setIsCoachSettingsWindowOpen(false);
        } else {
            ipcRenderer.send('open-coach-settings-window');
            setIsCoachSettingsWindowOpen(true);
        }
    };

    return { isCoachSettingsWindowOpen, toggleCoachSettingsWindow };
}
