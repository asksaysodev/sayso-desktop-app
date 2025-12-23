import { useSortable } from '@dnd-kit/sortable';
import { CSS } from '@dnd-kit/utilities';
import { Signal } from "../types";
import { LuGripVertical, LuTrash2 } from 'react-icons/lu';
import SignalCardField from './SignalCardField';
import SaysoModal from '@/components/SaysoModal';
import { useState } from 'react';
import CharacterCounter from './CharacterCounter';

interface Props {
    signal: Signal;
    handleOnChangeSignalField: (id: string, field: string, value: string | number) => void;
    handleDeleteSignal: (id: string) => void;
}

export default function SignalDraggable({ signal, handleOnChangeSignalField, handleDeleteSignal }: Props) {
    const [showDeleteModal, setShowDeleteModal] = useState(false);

    const {
        attributes,
        listeners,
        setNodeRef,
        transform,
        transition,
        isDragging,
    } = useSortable({
        id: signal.id,
    });

    const style = {
        transform: CSS.Transform.toString(transform),
        transition,
    };

    const handleKeyDown = (e: React.KeyboardEvent<HTMLInputElement>) => {
        if (e.key === 'ArrowUp' || e.key === 'ArrowDown') {
            e.preventDefault();
        }
    };

    return (
        <div 
            ref={setNodeRef} 
            style={style}
            className={`signal-draggable ${isDragging ? 'signal-dragging' : ''}`}
            {...attributes}
            {...listeners}
        >
            <div className='signal-draggable-left-fields'>
                <LuGripVertical size={22} color='var(--sayso-lightgray)' />
            </div>

            <div className='signal-draggable-center-fields'>
                <h3>{signal.name}</h3>
                <SignalCardField fieldName='Description'>
                    <div style={{ width: '100%' }}>
                        <textarea rows={3} value={signal.description} onChange={(e) => handleOnChangeSignalField(signal.id, 'description', e.target.value)} />
                        <CharacterCounter text={signal.description} type='signal_description' />
                    </div>
                </SignalCardField>
                <SignalCardField fieldName='Instruction'>
                    <div style={{ width: '100%' }}>
                        <textarea rows={3} value={signal.instructions} onChange={(e) => handleOnChangeSignalField(signal.id, 'instructions', e.target.value)} />
                        <CharacterCounter text={signal.instructions} type='signal_instructions' />
                    </div>
                </SignalCardField>
            </div>

            <div className='signal-draggable-right-fields'>
                <span />
                <div className='signal-draggable-right-fields-inputs'>
                    <SignalCardField fieldName='Priority'>
                        <div className='signal-draggable-priority'>
                            {signal.priority}
                        </div>
                    </SignalCardField>

                    <SignalCardField fieldName='Threshold'>
                        <input 
                            type='number' 
                            className='signal-draggable-threshold' 
                            value={signal.threshold} 
                            onChange={(e) => handleOnChangeSignalField(signal.id, 'threshold', e.target.value)}
                            onKeyDown={handleKeyDown}
                        />
                    </SignalCardField>
                </div>
                <button onClick={() => setShowDeleteModal(true)}>
                    <LuTrash2 size={18} color='var(--sayso-lightgray)' />
                </button>

                {showDeleteModal && (
                    <SaysoModal
                        title='Delete Signal'
                        text='This action cannot be undone. This will permanently delete the Signal.'
                        isDelete={true}
                        primaryText='Continue'
                        secondaryText='Cancel'
                        onConfirm={() => handleDeleteSignal(signal.id)}
                        onDeny={() => setShowDeleteModal(false)}
                    />
                )}
            </div>
        </div>
    )
}