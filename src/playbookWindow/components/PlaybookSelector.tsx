import { Playbook, PlaybookStatus } from '@/playbookWindow/types';

interface Props {
    playbooks: Playbook[];
    selectedId: string | null;
    onSelect: (id: string) => void;
}

const STATUS_LABEL: Record<PlaybookStatus, string> = {
    processing: 'Processing',
    ready: 'Ready',
    failed: 'Failed',
};

export default function PlaybookSelector({ playbooks, selectedId, onSelect }: Props) {
    if (playbooks.length === 0) {
        return (
            <div className="playbook-selector">
                <div className="playbook-selector-empty">No playbooks yet</div>
            </div>
        );
    }

    return (
        <ul className="playbook-selector" role="listbox">
            {playbooks.map((p) => {
                const isReady = p.status === 'ready';
                const isSelected = p.id === selectedId;
                const label = p.alias || p.file_name;
                return (
                    <li
                        key={p.id}
                        role="option"
                        aria-selected={isSelected}
                        aria-disabled={!isReady}
                        className={`playbook-selector-item ${isSelected ? 'selected' : ''} ${!isReady ? 'disabled' : ''}`}
                        onClick={() => isReady && onSelect(p.id)}
                    >
                        <span className="playbook-selector-item-label">{label}</span>
                        {!isReady && (
                            <span className={`playbook-status-pill status-${p.status}`}>
                                {STATUS_LABEL[p.status]}
                            </span>
                        )}
                    </li>
                );
            })}
        </ul>
    );
}
