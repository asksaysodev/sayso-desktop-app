import {
    DndContext,
    DragEndEvent,
    PointerSensor,
    closestCenter,
    useSensor,
    useSensors,
} from '@dnd-kit/core';
import { SortableContext, arrayMove, verticalListSortingStrategy } from '@dnd-kit/sortable';
import { Playbook } from '@/playbookWindow/types';
import { Spinner } from '@/components/ui/spinner';
import PlaybookRow from './PlaybookRow';

// Stable reference so useSensor/useSensors don't rebuild the sensor list
// (and its internal memoization) on every render.
const DRAG_ACTIVATION_CONSTRAINT = { distance: 4 };

interface ScriptsListProps {
    playbooks: Playbook[] | null;
    isLoading: boolean;
    queryError: string | null;
    onDelete: (id: string) => void;
    onUpdateAlias: (id: string, alias: string) => Promise<void> | void;
    deletingId: string | null;
    defaultPlaybookId: string | null;
    onSetDefault: (id: string) => void;
    onReorder: (order: string[]) => void;
}

export default function ScriptsList({
    playbooks,
    isLoading,
    queryError,
    onDelete,
    onUpdateAlias,
    deletingId,
    defaultPlaybookId,
    onSetDefault,
    onReorder,
}: ScriptsListProps) {
    const sensors = useSensors(useSensor(PointerSensor, { activationConstraint: DRAG_ACTIVATION_CONSTRAINT }));

    const handleDragEnd = (event: DragEndEvent) => {
        const { active, over } = event;
        if (!playbooks || !over || active.id === over.id) return;

        const oldIndex = playbooks.findIndex((p) => p.id === active.id);
        const newIndex = playbooks.findIndex((p) => p.id === over.id);
        if (oldIndex === -1 || newIndex === -1) return;

        const reordered = arrayMove(playbooks, oldIndex, newIndex);
        // Optimistic upload placeholders aren't real playbooks yet — never
        // persist their client-only temp- id as part of the saved order.
        onReorder(reordered.map((p) => p.id).filter((id) => !id.startsWith('temp-')));
    };

    return (
        <div id="playbooks-scripts" className="playbooks-list">

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
                        <span className="playbooks-col-handle" aria-hidden="true" />
                        <span className="playbooks-col-alias">NAME</span>
                        <span className="playbooks-col-owner">OWNER</span>
                        <span className="playbooks-col-status">STATUS</span>
                        <span className="playbooks-col-actions" aria-hidden="true" />
                    </div>
                    <DndContext sensors={sensors} collisionDetection={closestCenter} onDragEnd={handleDragEnd}>
                        <SortableContext items={playbooks.map((p) => p.id)} strategy={verticalListSortingStrategy}>
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
                        </SortableContext>
                    </DndContext>
                </div>
            )}
        </div>
    );
}
