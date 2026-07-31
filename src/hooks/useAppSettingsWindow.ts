import { useEffect, useState } from 'react';

export function useAppSettingsWindow() {
    const [isAppSettingsWindowOpen, setIsAppSettingsWindowOpen] = useState(false);

    useEffect(() => {
        const ipcRenderer = window.electron?.ipcRenderer;
        if (!ipcRenderer) return;

        const handleAppSettingsWindowState = (state: unknown) => {
            setIsAppSettingsWindowOpen((state as { isOpen: boolean }).isOpen);
        };

        const offAppSettingsWindowState = ipcRenderer.on('app-settings-window-state', handleAppSettingsWindowState);
        ipcRenderer.send('get-app-settings-window-state');

        return () => {
            offAppSettingsWindowState?.();
        };
    }, []);

    const toggleAppSettingsWindow = () => {
        const ipcRenderer = window.electron?.ipcRenderer;
        if (!ipcRenderer) return;

        if (isAppSettingsWindowOpen) {
            ipcRenderer.send('close-app-settings-window');
            setIsAppSettingsWindowOpen(false);
        } else {
            ipcRenderer.send('open-app-settings-window');
            setIsAppSettingsWindowOpen(true);
        }
    };

    return { isAppSettingsWindowOpen, toggleAppSettingsWindow };
}
