import { Info } from 'lucide-react';
import { PlaybookStatus } from '@/playbookWindow/types';
import { STATUS_LABELS } from '../constants';

interface StatusPillProps {
    status: PlaybookStatus;
    failureReason?: string | null;
}

export default function StatusPill({ status, failureReason }: StatusPillProps) {
    const showReason = status === 'failed' && !!failureReason;

    return (
        <span
            className={`playbooks-status-pill status-${status}`}
            title={showReason ? failureReason ?? undefined : undefined}
        >
            <span className="playbooks-status-dot" />
            {STATUS_LABELS[status]}
            {showReason && (
                <Info
                    size={12}
                    className="playbooks-status-info"
                    aria-label={failureReason ?? undefined}
                />
            )}
        </span>
    );
}
