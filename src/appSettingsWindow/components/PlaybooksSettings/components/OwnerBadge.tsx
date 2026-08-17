import { Lock } from 'lucide-react';
import { Playbook } from '@/playbookWindow/types';

// Only two owner tiers exist today. Organization (team-admin-uploaded,
// via the future admin panel) slots in here later as a third case —
// `PlaybookType` doesn't carry that value yet, so it isn't branched on.
function getOwnerLabel(playbook: Playbook): string {
    return playbook.type === 'custom' ? 'Private' : 'Sayso';
}

interface OwnerBadgeProps {
    playbook: Playbook;
}

export default function OwnerBadge({ playbook }: OwnerBadgeProps) {
    const isPrivate = playbook.type === 'custom';
    const label = getOwnerLabel(playbook);

    return (
        <span className={`playbooks-owner-badge ${isPrivate ? 'private' : 'sayso'}`}>
            {isPrivate ? (
                <Lock size={12} />
            ) : (
                <span className="playbooks-owner-icon playbooks-owner-icon-sayso" aria-hidden="true" />
            )}
            {label}
        </span>
    );
}
