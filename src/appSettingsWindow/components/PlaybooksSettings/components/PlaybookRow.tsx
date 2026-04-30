import { Star, Trash2 } from 'lucide-react';
import { Playbook } from '@/playbookWindow/types';
import StatusPill from './StatusPill';
import EditPlaybook from './EditPlaybook';

interface PlaybookRowProps {
    playbook: Playbook;
    isDeleting: boolean;
    isDefault: boolean;
    onDelete: () => void;
    onUpdateAlias: (alias: string) => Promise<void> | void;
    onSetDefault: () => void;
}

export default function PlaybookRow({ playbook, isDeleting, isDefault, onDelete, onUpdateAlias, onSetDefault }: PlaybookRowProps) {
    const isOptimistic = playbook.id.startsWith('temp-');
    const canDelete = !isOptimistic && !isDeleting;
    const canEdit = !isOptimistic && !isDeleting;
    const canSetDefault = !isOptimistic && !isDeleting && playbook.status === 'ready' && !isDefault;

    return (
        <div className={`playbooks-row ${isDeleting ? 'deleting' : ''}`}>
            <span className="playbooks-col-alias" title={playbook.alias ?? playbook.file_name}>
                <span className="playbooks-col-alias-text">{playbook.alias ?? playbook.file_name}</span>
                {canEdit && <EditPlaybook playbook={playbook} onSave={onUpdateAlias} />}
            </span>
            <span className="playbooks-col-file" title={playbook.file_name}>
                {playbook.file_name}
            </span>
            <span className="playbooks-col-status">
                <StatusPill status={playbook.status} />
            </span>
            <span className="playbooks-col-actions">
                <button
                    type="button"
                    className={`playbooks-row-star ${isDefault ? 'active' : ''}`}
                    onClick={canSetDefault ? onSetDefault : undefined}
                    disabled={!isDefault && playbook.status !== 'ready'}
                    aria-label={isDefault ? `Default playbook` : `Set ${playbook.alias ?? playbook.file_name} as default`}
                >
                    <Star
                        size={14}
                        fill={isDefault ? 'currentColor' : 'none'}
                    />
                </button>
                {canDelete && (
                    <button
                        type="button"
                        className="playbooks-row-delete"
                        onClick={onDelete}
                        aria-label={`Delete ${playbook.alias ?? playbook.file_name}`}
                    >
                        <Trash2 size={14} />
                    </button>
                )}
            </span>
        </div>
    );
}
