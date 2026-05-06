import LoginLayout from '@/components/layouts/LoginLayout';
import './styles.css';

interface Props {
    version: string | null;
    progress: number;
}

export default function Updating({ version, progress }: Props) {
    return (
        <LoginLayout
            title="Updating Sayso"
            description={`Downloading version ${version ?? ''}…`}
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
