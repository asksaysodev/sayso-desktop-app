import { RotateCw } from 'lucide-react';
import { Playbook } from '@/playbookWindow/types';
import { Spinner } from '@/components/ui/spinner';
import PlaybookRow from './PlaybookRow';

interface ScriptsListProps {
    playbooks: Playbook[] | null;
    isLoading: boolean;
    queryError: string | null;
    onRefresh: () => void;
    onDelete: (id: string) => void;
    onUpdateAlias: (id: string, alias: string) => Promise<void> | void;
    deletingId: string | null;
    defaultPlaybookId: string | null;
    onSetDefault: (id: string) => void;
}

export default function ScriptsList({
    playbooks,
    isLoading,
    queryError,
    onRefresh,
    onDelete,
    onUpdateAlias,
    deletingId,
    defaultPlaybookId,
    onSetDefault,
}: ScriptsListProps) {
    return (
        <div id="playbooks-scripts" className="playbooks-list">
            <div className="playbooks-list-header">
                <span className="playbooks-list-title">SCRIPTS</span>
                <button
                    type="button"
                    className="playbooks-refresh-btn"
                    onClick={onRefresh}
                    disabled={isLoading}
                    aria-label="Refresh playbooks"
                >
                    <RotateCw size={14} />
                </button>
            </div>

            {queryError && <p className="playbooks-list-error">{queryError}</p>}

            {isLoading && (!playbooks || playbooks.length === 0) ? (
                <div className="playbooks-list-state">
                    <Spinner width={16} height={16} />
                    <span>Loading…</span>
                </div>
            ) : !playbooks || playbooks.length === 0 ? (
                <div className="playbooks-list-state">
                    <span>No playbooks yet.</span>
                </div>
            ) : (
                <div className="playbooks-table">
                    <div className="playbooks-table-head">
                        <span className="playbooks-col-alias">ALIAS</span>
                        <span className="playbooks-col-file">FILE</span>
                        <span className="playbooks-col-status">STATUS</span>
                        <span className="playbooks-col-actions" />
                    </div>
                    {playbooks.map((p) => (
                        <PlaybookRow
                            key={p.id}
                            playbook={p}
                            isDeleting={deletingId === p.id}
                            isDefault={defaultPlaybookId === p.id}
                            onDelete={() => onDelete(p.id)}
                            onUpdateAlias={(alias) => onUpdateAlias(p.id, alias)}
                            onSetDefault={() => onSetDefault(p.id)}
                        />
                    ))}
                </div>
            )}
        </div>
    );
}
