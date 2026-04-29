import { PlaybookStatus } from '@/playbookWindow/types';
import { STATUS_LABELS } from '../constants';

interface StatusPillProps {
    status: PlaybookStatus;
}

export default function StatusPill({ status }: StatusPillProps) {
    return (
        <span className={`playbooks-status-pill status-${status}`}>
            <span className="playbooks-status-dot" />
            {STATUS_LABELS[status]}
        </span>
    );
}
