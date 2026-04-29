import { useEffect, useState } from 'react';
import * as DialogPrimitive from '@radix-ui/react-dialog';
import { SquarePen, X } from 'lucide-react';
import { Spinner } from '@/components/ui/spinner';
import { Playbook } from '@/playbookWindow/types';
import SaysoButton from '@/components/SaysoButton';

interface EditPlaybookProps {
    playbook: Playbook;
    onSave: (alias: string) => Promise<void> | void;
}

export default function EditPlaybook({ playbook, onSave }: EditPlaybookProps) {
    const initialAlias = playbook.alias ?? '';
    const [open, setOpen] = useState(false);
    const [alias, setAlias] = useState(initialAlias);
    const [isSaving, setIsSaving] = useState(false);
    const [error, setError] = useState<string | null>(null);

    useEffect(() => {
        if (open) {
            setAlias(initialAlias);
            setError(null);
        }
    }, [open, initialAlias]);

    const trimmed = alias.trim();
    const isUnchanged = trimmed === initialAlias.trim();
    const canSave = trimmed.length > 0 && !isUnchanged && !isSaving;

    const handleSave = async () => {
        if (!canSave) return;
        try {
            setIsSaving(true);
            setError(null);
            await onSave(trimmed);
            setOpen(false);
        } catch (e) {
            setError(e instanceof Error ? e.message : 'Failed to update playbook.');
        } finally {
            setIsSaving(false);
        }
    };

    return (
        <DialogPrimitive.Root open={open} onOpenChange={setOpen}>
            <DialogPrimitive.Trigger asChild>
                <button
                    type="button"
                    className="playbooks-edit-trigger"
                    aria-label={`Edit ${playbook.alias ?? playbook.file_name}`}
                >
                    <SquarePen size={14} />
                </button>
            </DialogPrimitive.Trigger>
            <DialogPrimitive.Portal>
                <DialogPrimitive.Overlay className="playbook-edit-overlay" />
                <DialogPrimitive.Content className="playbook-edit-dialog">
                    <div className="playbook-edit-header">
                        <DialogPrimitive.Title className="playbook-edit-title">
                            Edit Playbook
                        </DialogPrimitive.Title>
                        <DialogPrimitive.Description className="playbook-edit-description">
                            Update the alias for this playbook.
                        </DialogPrimitive.Description>
                    </div>

                    <div className="playbook-edit-fields">
                        <div className="playbook-edit-field">
                            <label className="playbook-edit-label" htmlFor="playbook-edit-alias">
                                Alias
                            </label>
                            <input
                                id="playbook-edit-alias"
                                className="playbook-edit-input"
                                type="text"
                                value={alias}
                                onChange={(e) => setAlias(e.target.value)}
                                disabled={isSaving}
                                autoFocus
                            />
                        </div>

                        <div className="playbook-edit-field">
                            <label className="playbook-edit-label" htmlFor="playbook-edit-file">
                                File
                            </label>
                            <input
                                id="playbook-edit-file"
                                className="playbook-edit-input is-disabled"
                                type="text"
                                value={playbook.file_name}
                                disabled
                                readOnly
                            />
                        </div>
                    </div>

                    {error && <p className="playbook-edit-error">{error}</p>}

                    <div className="playbook-edit-footer">
                        <DialogPrimitive.Close asChild>
                            <SaysoButton
                                label={'Cancel'}
                                variant='transparent-border'
                                disabled={isSaving}
                            />
                        </DialogPrimitive.Close>
                        
                        <SaysoButton 
                            label={'Save'}
                            variant='blue'
                            onClick={handleSave}
                            disabled={!canSave}
                            loading={isSaving}             
                        />
                    </div>

                    <DialogPrimitive.Close
                        className="playbook-edit-close"
                        aria-label="Close"
                    >
                        <X size={16} />
                    </DialogPrimitive.Close>
                </DialogPrimitive.Content>
            </DialogPrimitive.Portal>
        </DialogPrimitive.Root>
    );
}
