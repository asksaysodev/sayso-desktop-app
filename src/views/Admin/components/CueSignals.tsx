import { useState, useEffect, useMemo } from "react";
import SelectLeadType from "./SelectLeadType";
import { useMutation, useQuery } from "@tanstack/react-query";
import getSignals from "../services/getSignals";
import { 
    DndContext, 
    DragEndEvent, 
    closestCenter, 
    PointerSensor, 
    useSensor, 
    useSensors 
} from '@dnd-kit/core';
import { 
    SortableContext, 
    verticalListSortingStrategy,
    arrayMove 
} from '@dnd-kit/sortable';
import SignalsDraggableList from "./SignalsDraggableList";
import { Signal } from "../types";
import { useAdminStore } from "@/store/adminStore";
import updateSignalsByLeadType from "../services/updateSignalsByLeadType";
import { useToast } from "@/context/ToastContext";
import ButtonSpinner from "@/components/ButtonSpinner";

function updatePriorities(signals: Signal[]): Signal[] {
    return signals.map((signal, index) => ({
        ...signal,
        priority: index + 1
    }));
}

export default function CueSignals() {
    const [originalSignals, setOriginalSignals] = useState<Signal[]>([]);
    const [newSignals, setNewSignals] = useState<Signal[]>([]);
    const leadType = useAdminStore(state => state.leadType);
    const { showToast } = useToast();
    
    const sensors = useSensors(
        useSensor(PointerSensor, {
            activationConstraint: {
                distance: 8,
            },
        })
    );

    const { mutate: updateSignals, isPending: isUpdatingSignals } = useMutation({
        mutationFn: (uniqueSignals: Signal[]) => updateSignalsByLeadType(leadType, uniqueSignals),
        onSuccess: (data) => {
            showToast('success', 'Signals updated successfully!');
            setOriginalSignals(data?.signals);
            setNewSignals(data?.signals);
        },
        onError: () => {
            showToast('error', 'Failed to update signals');
        },
    });

    const { data: signals, isLoading: isLoadingSignals, error: errorSignals, refetch: refetchSignals, isRefetching: isRefetchingSignals } = useQuery({
        queryKey: ['cue-signals', leadType],
        queryFn: () => getSignals(leadType),
    });

    useEffect(() => {
        if (signals) {
            const signalsWithPriorities = updatePriorities(signals);
            setOriginalSignals(signalsWithPriorities);
            setNewSignals(signalsWithPriorities);
        }
    }, [signals]);

    const handleDragEnd = (event: DragEndEvent) => {
        const { active, over } = event;

        if (!over || active.id === over.id) {
            return;
        }

        setNewSignals((signals) => {
            const oldIndex = signals.findIndex((s) => s.id === active.id);
            const newIndex = signals.findIndex((s) => s.id === over.id);
            const reordered = arrayMove(signals, oldIndex, newIndex);
            
            return updatePriorities(reordered);
        });
    };

    const handleReset = () => {
        setNewSignals(originalSignals);
    };

    const handleOnChangeSignalField = (id: string, field: string, value: string | number) => {
        setNewSignals((signals) => {
            return signals.map((signal) => {
                if (signal.id === id) {
                    return { ...signal, [field]: value };
                }
                return signal;
            });
        });
    }

    const handleDeleteSignal = (id: string) => {
        setNewSignals((prev) => {
            const filtered = prev.filter((signal) => signal.id !== id);
            return updatePriorities(filtered);
        });
    };

    const handleSubmit = () => {
        const seenIds = new Set<string>();
        const uniqueSignals = newSignals.filter((signal) => {
            if (seenIds.has(signal.id)) {
                return false;
            }
            seenIds.add(signal.id);
            return true;
        }).map((signal) => {
            if (signal.id.includes('temporaryid')) {
                const { id, ...signalWithoutId } = signal;
                return signalWithoutId;
            }
            return signal;
        });
        updateSignals(uniqueSignals as Signal[]);
    }

    const hasChanges = useMemo(() => JSON.stringify(originalSignals) !== JSON.stringify(newSignals), [originalSignals, newSignals]);

    const handleSubmitAddSignal = (data: Signal) => {
        const id = `temporaryid-${data.name}-${Date.now()}`;
        const newData = {
            ...data,
            id
        }
        const insertIndex = data.priority - 1;
        const updatedSignals = [...newSignals];
        updatedSignals.splice(insertIndex, 0, newData);
        const saveSignals = updatePriorities(updatedSignals) ?? [];
        setNewSignals(saveSignals);
    }

    return (
        <DndContext 
            sensors={sensors}
            collisionDetection={closestCenter}
            onDragEnd={handleDragEnd}
        >
            <SortableContext 
                items={newSignals.map(s => s.id)}
                strategy={verticalListSortingStrategy}
            >
                <SignalsDraggableList 
                    signals={newSignals} 
                    handleOnChangeSignalField={handleOnChangeSignalField} 
                    handleDeleteSignal={handleDeleteSignal} 
                    handleSubmitAddSignal={handleSubmitAddSignal}
                    isLoadingSignals={isLoadingSignals}
                    error={errorSignals}
                    refetchSignals={refetchSignals}
                    isRefetchingSignals={isRefetchingSignals}
                />
            </SortableContext>

            {hasChanges && (
                <div className='signal-actions'>
                    <button 
                        disabled={isUpdatingSignals} 
                        onClick={handleReset} 
                        className='signal-action-btn signal-reset-btn'
                        style={{ opacity: isUpdatingSignals ? 0.5 : 1 }}
                    >
                        Reset
                    </button>
                    <button 
                        disabled={isUpdatingSignals} 
                        onClick={handleSubmit} 
                        className='signal-action-btn signal-submit-btn'
                        style={{ 
                            opacity: isUpdatingSignals ? 0.5 : 1,
                            display: 'flex',
                            alignItems: 'center',
                            gap: '8px',
                            cursor: isUpdatingSignals ? 'not-allowed' : 'pointer'
                        }}
                    >
                        {isUpdatingSignals && <ButtonSpinner />}
                        Save Changes
                    </button>
                </div>
            )}
        </DndContext>
    )
}