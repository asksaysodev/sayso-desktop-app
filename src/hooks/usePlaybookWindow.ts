import { useEffect, useState } from 'react';

export function usePlaybookWindow() {
    const [isPlaybookWindowOpen, setIsPlaybookWindowOpen] = useState(false);

    useEffect(() => {
        const ipcRenderer = window.electron?.ipcRenderer;
        if (!ipcRenderer) return;

        const handlePlaybookWindowState = (state: unknown) => {
            setIsPlaybookWindowOpen((state as { isOpen: boolean }).isOpen);
        };

        ipcRenderer.on('playbook-window-state', handlePlaybookWindowState);
        ipcRenderer.send('get-playbook-window-state');

        return () => {
            ipcRenderer.off('playbook-window-state', handlePlaybookWindowState);
        };
    }, []);

    const togglePlaybookWindow = () => {
        const ipcRenderer = window.electron?.ipcRenderer;
        if (!ipcRenderer) return;

        if (isPlaybookWindowOpen) {
            ipcRenderer.send('close-playbook-window');
        } else {
            ipcRenderer.send('open-playbook-window');
        }
    };

    return { isPlaybookWindowOpen, togglePlaybookWindow };
}
