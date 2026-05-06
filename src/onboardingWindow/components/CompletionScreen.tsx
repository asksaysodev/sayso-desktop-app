import { LuSettings, LuRefreshCw } from 'react-icons/lu';

interface Props {
    onRepeat: () => void;
}

export default function CompletionScreen({ onRepeat }: Props) {
    const handleOpenSettings = () => {
        window.electron?.ipcRenderer?.send('open-app-settings-window');
        window.electron?.ipcRenderer?.send('complete-onboarding');
    };

    const handleDone = () => {
        window.electron?.ipcRenderer?.send('complete-onboarding');
    };

    return (
        <div className="onboarding-window completion-screen">
            <div className="onboarding-drag-bar" />
            <div className="completion-content">
                <div className="completion-checkmark">
                    <svg viewBox="0 0 52 52" width="52" height="52">
                        <circle
                            className="completion-checkmark-circle"
                            cx="26" cy="26" r="23"
                            fill="none"
                            stroke="#2aab13"
                            strokeWidth="2.5"
                        />
                        <path
                            className="completion-checkmark-path"
                            fill="none"
                            stroke="#2aab13"
                            strokeWidth="3"
                            strokeLinecap="round"
                            strokeLinejoin="round"
                            d="M14 26 L22 34 L38 18"
                        />
                    </svg>
                </div>
                <h1 className="onboarding-title">You're all set!</h1>
                <p className="onboarding-subtitle">
                    Customize your coach behavior, size, and more in Settings for a tailored experience.
                </p>
                <div className="completion-actions">
                    <button className="completion-btn completion-btn--secondary" onClick={onRepeat}>
                        <LuRefreshCw size={14} />
                        Watch Again
                    </button>
                    <button className="completion-btn completion-btn--primary" onClick={handleOpenSettings}>
                        <LuSettings size={14} />
                        Open Settings
                    </button>
                </div>
            </div>
            <div className="onboarding-footer">
                <span />
                <span />
                <button className="onboarding-next" onClick={handleDone}>
                    Done
                </button>
            </div>
        </div>
    );
}
