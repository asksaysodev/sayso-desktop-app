import { ExternalLink } from 'lucide-react';

interface Props {
    highlightLaunchCoach?: boolean;
    step1?: boolean;
}

export default function TrayMenuMock({ highlightLaunchCoach = false, step1 = false }: Props) {
    return (
        <div className={`tray-menu-mock${step1 ? ' tray-menu-mock--step1' : ''}`}>
            <div className="tmm-header">
                <span className="tmm-title">Sayso</span>
            </div>
            <div className="tmm-items">
                <div className={`tmm-item ${highlightLaunchCoach ? 'tmm-item--highlighted' : ''}`}>
                    <span className="tmm-item-label">Launch Coach</span>
                    <span className="tmm-shortcut">Ctrl + S</span>
                </div>
                <div className="tmm-separator" />
                <div className="tmm-item">
                    <span className="tmm-item-label">My Account</span>
                    <ExternalLink size={14} className="tmm-item-icon" />
                </div>
                <div className="tmm-separator" />
                <div className="tmm-item">
                    <span className="tmm-item-label">Settings</span>
                </div>
                <div className="tmm-separator" />
                <div className="tmm-bottom-row">
                    <div className="tmm-item tmm-item--bottom">
                        <span>Quit</span>
                    </div>
                    <div className="tmm-item tmm-item--bottom">
                        <span>Log Out</span>
                    </div>
                </div>
            </div>
        </div>
    );
}
