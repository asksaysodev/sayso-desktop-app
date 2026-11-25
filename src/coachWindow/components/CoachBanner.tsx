import { forwardRef } from 'react';
import { CircleAlert, TriangleAlert, WifiOff, X } from 'lucide-react';
import { FaPause } from 'react-icons/fa6';
import type { CoachError, CoachErrorIcon } from '@/types/store/coachWindowStore';
import '../styles/CoachBanner.css';

const ICONS = {
    pause: FaPause,
    offline: WifiOff,
} satisfies Record<CoachErrorIcon, unknown>;

interface Props extends Pick<CoachError, 'title' | 'message' | 'severity' | 'icon'> {
    action?: { label: string; onClick: () => void; disabled?: boolean };
    onDismiss?: () => void;
}

const CoachBanner = forwardRef<HTMLDivElement, Props>(({
    title, message, severity = 'error', icon, action, onDismiss,
}, ref) => {
    const DefaultIcon = severity === 'error' ? CircleAlert : TriangleAlert;
    const Icon = icon ? ICONS[icon] : DefaultIcon;

    return (
        <div
            ref={ref}
            className={`coach-banner coach-banner--${severity} coach-box-bubble`}
            role={severity === 'error' ? 'alert' : 'status'}
        >
            <span className="coach-banner-icon" aria-hidden="true">
                <Icon size={16} />
            </span>
            <p className="coach-banner-copy">
                {title && <span className="coach-banner-title">{title} </span>}
                <span className={title ? 'coach-banner-detail' : undefined}>{message}</span>
            </p>
            {action && (
                <button className="coach-banner-action" onClick={action.onClick} disabled={action.disabled}>
                    {action.label}
                </button>
            )}
            {onDismiss && (
                <button className="coach-banner-dismiss" onClick={onDismiss} aria-label="Dismiss message">
                    <X size={14} />
                </button>
            )}
        </div>
    );
});

export default CoachBanner;
