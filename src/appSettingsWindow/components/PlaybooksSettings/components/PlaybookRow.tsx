import { GripVertical, Star, Trash2 } from 'lucide-react';
import { useSortable } from '@dnd-kit/sortable';
import { CSS } from '@dnd-kit/utilities';
import { Playbook } from '@/playbookWindow/types';
import StatusPill from './StatusPill';
import EditPlaybook from './EditPlaybook';
import OwnerBadge from './OwnerBadge';

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
    const isReady = playbook.status === 'ready';
    const canDelete = !isOptimistic && !isDeleting && isReady && playbook.type === 'custom';
    const canEdit = !isOptimistic && !isDeleting && isReady && playbook.type === 'custom';
    const canSetDefault = !isOptimistic && !isDeleting && isReady && !isDefault;

    const { attributes, listeners, setNodeRef, transform, transition, isDragging } = useSortable({
        id: playbook.id,
        disabled: isOptimistic || isDeleting,
    });

    const style: React.CSSProperties = {
        transform: CSS.Transform.toString(transform),
        transition,
    };

    return (
        <div
            ref={setNodeRef}
            style={style}
            className={`playbooks-row ${isDeleting ? 'deleting' : ''} ${isDragging ? 'dragging' : ''}`}
        >
            <span className="playbooks-col-handle">
                <button
                    type="button"
                    className="playbooks-row-drag-handle"
                    disabled={isOptimistic || isDeleting}
                    aria-label={`Reorder ${playbook.alias ?? playbook.file_name}`}
                    {...attributes}
                    {...listeners}
                >
                    <GripVertical size={24} />
                </button>
            </span>
            <span className="playbooks-col-alias" title={playbook.file_name}>
                <span className="playbooks-col-alias-text">{playbook.alias ?? playbook.file_name}</span>
            </span>
            <span className="playbooks-col-owner">
                <OwnerBadge playbook={playbook} />
            </span>
            <span className="playbooks-col-status">
                <StatusPill status={playbook.status} failureReason={playbook.failure_reason} />
            </span>
            <span className="playbooks-col-actions">
                {canEdit && <EditPlaybook playbook={playbook} onSave={onUpdateAlias} />}
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
                {isReady && (
                    <button
                        type="button"
                        className={`playbooks-row-star ${isDefault ? 'active' : ''}`}
                        onClick={canSetDefault ? onSetDefault : undefined}
                        disabled={!isDefault && !canSetDefault}
                        aria-label={isDefault ? `Default playbook` : `Set ${playbook.alias ?? playbook.file_name} as default`}
                    >
                        <Star
                            size={14}
                            fill={isDefault ? 'currentColor' : 'none'}
                        />
                    </button>
                )}
            </span>
        </div>
    );
}
