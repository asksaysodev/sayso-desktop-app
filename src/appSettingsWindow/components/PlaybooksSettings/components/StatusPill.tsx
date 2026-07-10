import { useRef, useState } from 'react';
import { Info } from 'lucide-react';
import { PlaybookStatus } from '@/playbookWindow/types';
import { STATUS_LABELS } from '../constants';

interface StatusPillProps {
    status: PlaybookStatus;
    failureReason?: string | null;
}

export default function StatusPill({ status, failureReason }: StatusPillProps) {
    const showReason = status === 'failed' && !!failureReason;
    const iconRef = useRef<HTMLSpanElement>(null);
    // Fixed-position coords computed from the icon so the tooltip escapes the
    // scrollable list container instead of clipping on the top/bottom rows.
    const [coords, setCoords] = useState<{ top: number; left: number } | null>(null);

    const handleEnter = () => {
        const rect = iconRef.current?.getBoundingClientRect();
        if (!rect) return;
        setCoords({ top: rect.top - 6, left: rect.right });
    };

    return (
        <span className={`playbooks-status-pill status-${status}`}>
            <span className="playbooks-status-dot" />
            {STATUS_LABELS[status]}
            {showReason && (
                <span
                    ref={iconRef}
                    className="playbooks-status-info"
                    aria-label={failureReason ?? undefined}
                    onMouseEnter={handleEnter}
                    onMouseLeave={() => setCoords(null)}
                >
                    <Info size={12} />
                    {coords && (
                        <span
                            className="playbooks-status-tooltip"
                            style={{ top: coords.top, left: coords.left }}
                        >
                            {failureReason}
                        </span>
                    )}
                </span>
            )}
        </span>
    );
}
