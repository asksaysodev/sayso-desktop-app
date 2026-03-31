import { useState, useEffect, ReactNode } from 'react';
import SaysoLoader from '@/components/SaysoLoader';
import Updating from './routes/Updating';

interface Props {
    children: ReactNode;
}

const UPDATE_CHECK_TIMEOUT_MS = 15000;

export default function UpdateGate({ children }: Props) {
    const [updateCheckDone, setUpdateCheckDone] = useState(import.meta.env.DEV);
    const [updateAvailable, setUpdateAvailable] = useState(false);
    const [updateVersion, setUpdateVersion] = useState<string | null>(null);
    const [downloadProgress, setDownloadProgress] = useState(0);
    const [updateDownloaded, setUpdateDownloaded] = useState(false);

    useEffect(() => {
        if (import.meta.env.DEV) return;

        const timeout = setTimeout(() => {
            setUpdateCheckDone(true);
        }, UPDATE_CHECK_TIMEOUT_MS);

        const cleanupCheckComplete = window.electron?.autoUpdater?.onUpdateCheckComplete(() => {
            clearTimeout(timeout);
            setUpdateCheckDone(true);
        });

        const cleanupAvailable = window.electron?.autoUpdater?.onUpdateAvailable((data) => {
            clearTimeout(timeout);
            setUpdateCheckDone(true);
            setUpdateAvailable(true);
            setUpdateVersion(data.version);
        });

        const cleanupProgress = window.electron?.autoUpdater?.onDownloadProgress((data) => {
            setDownloadProgress(data.percent);
        });

        const cleanupDownloaded = window.electron?.autoUpdater?.onUpdateDownloaded((data) => {
            setUpdateDownloaded(true);
            setUpdateVersion(data.version);
        });

        return () => {
            clearTimeout(timeout);
            cleanupCheckComplete?.();
            cleanupAvailable?.();
            cleanupProgress?.();
            cleanupDownloaded?.();
        };
    }, []);

    if (!updateCheckDone) return <SaysoLoader />;

    if (updateAvailable) {
        return (
            <Updating
                version={updateVersion}
                progress={downloadProgress}
                downloaded={updateDownloaded}
                onDismiss={() => setUpdateAvailable(false)}
            />
        );
    }

    return <>{children}</>;
}
