import { useEffect } from 'react';
import { useMutation, useQuery, useQueryClient } from '@tanstack/react-query';
import * as Sentry from '@sentry/electron/renderer';
import {
    deletePlaybook,
    getPlaybooks,
    updatePlaybookAlias,
    uploadPlaybook,
} from '@/playbookWindow/services/playbookServices';
import { Playbook, PlaybooksCachePayload } from '@/playbookWindow/types';
import SettingsContentLayout from '../SettingsContentLayout';
import SettingsToggle from '../SettingsToggle';
import UploadCard from './components/UploadCard';
import ScriptsList from './components/ScriptsList';
import { PLAYBOOK_LIMIT, PLAYBOOKS_QUERY_KEY, getErrorMessage } from './constants';
import useCoachSettingsContext from '../../context/CoachSettingsContext';

export default function PlaybooksSettings() {
    const queryClient = useQueryClient();
    const { coachSettings, mutateDefaultPlaybook, mutateOpenLastUsed } = useCoachSettingsContext();
    const { data: playbooks, error: queryError, isLoading } = useQuery({
        queryKey: PLAYBOOKS_QUERY_KEY,
        queryFn: getPlaybooks,
    });

    useEffect(() => {
        if (isLoading) return;
        const payload: PlaybooksCachePayload = {
            playbooks: playbooks ?? null,
            error: queryError ? getErrorMessage(queryError) : null,
        };
        window.electron?.ipcRenderer?.send('set-playbooks-cache', payload);
    }, [playbooks, queryError, isLoading]);

    const customCount = (playbooks ?? []).filter((p) => p.type === 'custom').length;
    const isAtLimit = customCount >= PLAYBOOK_LIMIT;

    const uploadMutation = useMutation({
        mutationFn: ({ file, alias }: { file: File; alias: string }) =>
            uploadPlaybook(file, alias),
        onMutate: async ({ file, alias }) => {
            await queryClient.cancelQueries({ queryKey: PLAYBOOKS_QUERY_KEY });
            const prev = queryClient.getQueryData<Playbook[]>(PLAYBOOKS_QUERY_KEY);
            const tempId = `temp-${Date.now()}`;
            const optimistic: Playbook = {
                id: tempId,
                account_id: '',
                alias,
                file_name: file.name,
                file_size: file.size,
                file_type: file.type,
                status: 'processing',
                type: 'custom',
                blocks: null,
                is_default: false,
            };
            queryClient.setQueryData<Playbook[]>(
                PLAYBOOKS_QUERY_KEY,
                [...(prev ?? []), optimistic],
            );
            return { prev, tempId };
        },
        onError: (err, _vars, context) => {
            if (context?.prev !== undefined) {
                queryClient.setQueryData(PLAYBOOKS_QUERY_KEY, context.prev);
            }
            Sentry.captureException(err);
        },
        onSuccess: (data, _vars, context) => {
            queryClient.setQueryData<Playbook[]>(PLAYBOOKS_QUERY_KEY, (prev) => {
                if (!prev) return [data];
                return prev.map((p) => (p.id === context?.tempId ? data : p));
            });
        },
        onSettled: () => {
            queryClient.invalidateQueries({ queryKey: PLAYBOOKS_QUERY_KEY });
        },
    });

    const deleteMutation = useMutation({
        mutationFn: (id: string) => deletePlaybook(id),
        onMutate: async (id) => {
            await queryClient.cancelQueries({ queryKey: PLAYBOOKS_QUERY_KEY });
            const prev = queryClient.getQueryData<Playbook[]>(PLAYBOOKS_QUERY_KEY);
            queryClient.setQueryData<Playbook[]>(
                PLAYBOOKS_QUERY_KEY,
                (prev ?? []).filter((p) => p.id !== id),
            );
            return { prev };
        },
        onError: (err, _id, context) => {
            if (context?.prev !== undefined) {
                queryClient.setQueryData(PLAYBOOKS_QUERY_KEY, context.prev);
            }
            Sentry.captureException(err);
        },
        onSettled: () => {
            queryClient.invalidateQueries({ queryKey: PLAYBOOKS_QUERY_KEY });
        },
    });

    const updateAliasMutation = useMutation({
        mutationFn: ({ id, alias }: { id: string; alias: string }) =>
            updatePlaybookAlias(id, alias),
        onMutate: async ({ id, alias }) => {
            await queryClient.cancelQueries({ queryKey: PLAYBOOKS_QUERY_KEY });
            const prev = queryClient.getQueryData<Playbook[]>(PLAYBOOKS_QUERY_KEY);
            queryClient.setQueryData<Playbook[]>(
                PLAYBOOKS_QUERY_KEY,
                (prev ?? []).map((p) => (p.id === id ? { ...p, alias } : p)),
            );
            return { prev };
        },
        onError: (err, _vars, context) => {
            if (context?.prev !== undefined) {
                queryClient.setQueryData(PLAYBOOKS_QUERY_KEY, context.prev);
            }
            Sentry.captureException(err);
        },
        onSettled: () => {
            queryClient.invalidateQueries({ queryKey: PLAYBOOKS_QUERY_KEY });
        },
    });

    const handleRefresh = () => {
        queryClient.invalidateQueries({ queryKey: PLAYBOOKS_QUERY_KEY });
    };

    return (
        <SettingsContentLayout
            title="Playbooks"
            description="Upload and manage call scripts. One script can be set as default for new sessions."
        >
            <div id="open-last-used" className="cue-setting-item">
                <div className="cue-setting-left">
                    <span className="cue-setting-label">Open Last Used</span>
                    <p className="cue-setting-description">Reopen the playbook you used most recently instead of the default</p>
                </div>
                <div className="cue-setting-right">
                    <SettingsToggle
                        options={[
                            { value: 'disabled', label: 'Disabled' },
                            { value: 'enabled', label: 'Enabled' },
                        ]}
                        value={coachSettings?.open_last_used !== false ? 'enabled' : 'disabled'}
                        onChange={(value) => mutateOpenLastUsed(value === 'enabled')}
                    />
                </div>
            </div>

            <UploadCard
                disabled={isAtLimit}
                isUploading={uploadMutation.isPending}
                uploadError={
                    uploadMutation.isError ? getErrorMessage(uploadMutation.error) : null
                }
                onUpload={(file, alias) => uploadMutation.mutate({ file, alias })}
                onClearError={() => uploadMutation.reset()}
            />

            <ScriptsList
                playbooks={playbooks ?? null}
                isLoading={isLoading}
                queryError={queryError ? getErrorMessage(queryError) : null}
                onRefresh={handleRefresh}
                onDelete={(id) => deleteMutation.mutate(id)}
                onUpdateAlias={(id, alias) =>
                    updateAliasMutation.mutateAsync({ id, alias })
                }
                deletingId={deleteMutation.isPending ? deleteMutation.variables ?? null : null}
                defaultPlaybookId={coachSettings?.default_playbook_id ?? null}
                onSetDefault={(id) => mutateDefaultPlaybook(id)}
            />
        </SettingsContentLayout>
    );
}
