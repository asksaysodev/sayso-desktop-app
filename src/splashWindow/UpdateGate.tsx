import { useState, useEffect, ReactNode } from 'react';
import { useNavigate } from 'react-router-dom';
import { useAuth } from '@/context/AuthContext';
import SaysoLoader from '@/components/SaysoLoader';
import Updater from './routes/Updater';
import Updating from './routes/Updating';
import { UpdateState } from '@/types/update';

interface Props {
    children: ReactNode;
}

const UPDATE_CHECK_TIMEOUT_MS = 15000;

export default function UpdateGate({ children }: Props) {
    const [updateCheckDone, setUpdateCheckDone] = useState(import.meta.env.DEV);
    const [updateState, setUpdateState] = useState<UpdateState | null>(null);
    const { user } = useAuth();
    const navigate = useNavigate();

    useEffect(() => {
        if (import.meta.env.DEV) return;

        const timeout = setTimeout(() => setUpdateCheckDone(true), UPDATE_CHECK_TIMEOUT_MS);

        // Hydrate from main on mount — if update is already available, show immediately
        window.electron?.update?.getState().then((state) => {
            if (state.phase !== 'idle') {
                setUpdateState(state);
                setUpdateCheckDone(true);
                clearTimeout(timeout);
            }
        });

        const cleanupState = window.electron?.update?.onStateChanged((state) => {
            setUpdateState(state);
            if (state.phase !== 'idle') {
                setUpdateCheckDone(true);
                clearTimeout(timeout);
            }
        });

        // Fallback: update-check-complete fires when no update is available
        const cleanupCheckComplete = window.electron?.autoUpdater?.onUpdateCheckComplete(() => {
            clearTimeout(timeout);
            setUpdateCheckDone(true);
        });

        return () => {
            clearTimeout(timeout);
            cleanupState?.();
            cleanupCheckComplete?.();
        };
    }, []);

    const handleUpdate = () => {
        window.electron?.update?.startDownload();
    };

    const handleDismiss = () => {
        window.electron?.update?.dismiss();
        if (user) {
            window.electron?.ipcRenderer?.send('splash-login-success');
        } else {
            navigate('/login');
        }
    };

    if (!updateCheckDone) return <SaysoLoader />;

    const phase = updateState?.phase;

    if (phase === 'available') {
        return (
            <Updater
                currentVersion={updateState!.currentVersion}
                newVersion={updateState!.newVersion!}
                onUpdate={handleUpdate}
                onDismiss={handleDismiss}
            />
        );
    }

    if (phase === 'downloading') {
        return (
            <Updating
                version={updateState!.newVersion}
                progress={updateState!.progressPercent}
            />
        );
    }

    return <>{children}</>;
}
