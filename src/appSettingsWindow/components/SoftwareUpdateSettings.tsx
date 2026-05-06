import { useEffect, useState } from 'react';
import * as Sentry from '@sentry/electron/renderer';
import { UpdateState } from '@/types/update';
import SettingsContentLayout from './SettingsContentLayout';
import './SoftwareUpdateSettings.css';

export default function SoftwareUpdateSettings() {
    const [state, setState] = useState<UpdateState | null>(null);

    useEffect(() => {
        window.electron?.update?.getState().then(setState).catch(Sentry.captureException);
        const cleanup = window.electron?.update?.onStateChanged(setState);
        return cleanup;
    }, []);

    const handleUpdateNow = () => {
        window.electron?.update?.startDownload();
    };

    const handleCheckForUpdates = () => {
        window.electron?.update?.checkForUpdates();
    };

    const currentVersion = state?.currentVersion ?? '—';
    const newVersion = state?.newVersion;
    const phase = state?.phase ?? 'idle';
    const progress = state?.progressPercent ?? 0;

    const renderStatus = () => {
        switch (phase) {
            case 'available':
                return (
                    <div className="su-status-block">
                        <div className="su-version-row">
                            <span className="su-version-pill su-version-current">v{currentVersion}</span>
                            <span className="su-version-arrow">→</span>
                            <span className="su-version-pill su-version-new">v{newVersion}</span>
                        </div>
                        <p className="su-description">A new version of Sayso is ready to install.</p>
                        <button className="su-update-btn" onClick={handleUpdateNow}>
                            Update Now
                        </button>
                    </div>
                );
            case 'downloading':
                return (
                    <div className="su-status-block">
                        <p className="su-status-label">Downloading update… {Math.round(progress)}%</p>
                        <div className="su-progress-track">
                            <div className="su-progress-fill" style={{ width: `${progress}%` }} />
                        </div>
                        {newVersion && (
                            <div className="su-version-row" style={{ marginTop: 12 }}>
                                <span className="su-version-pill su-version-current">v{currentVersion}</span>
                                <span className="su-version-arrow">→</span>
                                <span className="su-version-pill su-version-new">v{newVersion}</span>
                            </div>
                        )}
                    </div>
                );
            case 'downloaded':
                return (
                    <div className="su-status-block">
                        <p className="su-status-label">Applying update, restarting…</p>
                    </div>
                );
            case 'error':
                return (
                    <div className="su-status-block">
                        <p className="su-status-label su-status-error">
                            {state?.errorMessage ?? 'Update failed. Please try again.'}
                        </p>
                        <button className="su-update-btn" onClick={handleCheckForUpdates}>
                            Try Again
                        </button>
                    </div>
                );
            default:
                return (
                    <div className="su-status-block">
                        <p className="su-status-label">Sayso v{currentVersion} is up to date.</p>
                        <button className="su-check-btn" onClick={handleCheckForUpdates}>
                            Check for Updates
                        </button>
                    </div>
                );
        }
    };

    return (
        <SettingsContentLayout title="Software Update">
            <div id="software-update" className="su-setting-item">
                {renderStatus()}
            </div>
        </SettingsContentLayout>
    );
}
