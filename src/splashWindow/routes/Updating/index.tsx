import LoginLayout from '@/components/layouts/LoginLayout';
import LoginBtn from '@/components/LoginBtn';
import './styles.css';

interface Props {
    version: string | null;
    progress: number;
    downloaded: boolean;
    onDismiss: () => void;
}

export default function Updating({ version, progress, downloaded, onDismiss }: Props) {
    const handleInstall = () => {
        window.electron?.ipcRenderer?.send('install-update');
    };

    if (downloaded) {
        return (
            <LoginLayout
                title="Update Ready"
                description={`Version ${version} has been downloaded and is ready to install.`}
            >
                <div className="updating-actions">
                    <LoginBtn text="Restart Now" onClick={handleInstall} />
                    <LoginBtn text="Later" onClick={onDismiss} isSecondary />
                </div>
            </LoginLayout>
        );
    }

    return (
        <LoginLayout
            title="Updating Sayso"
            description={`Downloading version ${version ?? ''}...`}
        >
            <div className="updating-progress">
                <div className="updating-progress-bar">
                    <div
                        className="updating-progress-fill"
                        style={{ width: `${progress}%` }}
                    />
                </div>
                <p className="updating-progress-label">{Math.round(progress)}%</p>
            </div>
        </LoginLayout>
    );
}
