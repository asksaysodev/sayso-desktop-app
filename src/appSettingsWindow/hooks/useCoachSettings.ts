import { useMutation, useQuery, useQueryClient } from "@tanstack/react-query";
import * as Sentry from "@sentry/electron/renderer";
import { useEffect } from "react";
import postBufferTime from "../services/cue/postBufferTime";
import getCoachSettings from "../services/cue/getCoachSettings";
import updateCueMode from "../services/cue/updateCueInsightMode";
import postAutoStopTimeDelay from "../services/cue/postAutoStopTimeDelay";
import updateFontSize from "../services/accessibility/updateFontSize";
import postOpenLastUsed from "../services/cue/postOpenLastUsed";
import postPulseEnabled from "../services/cue/postPulseEnabled";
import { setDefaultPlaybook, updatePlaybookOrder } from "@/playbookWindow/services/playbookServices";
import { GetCoachSettingsResponse } from "../types";
import { Playbook } from "@/playbookWindow/types";
import { PLAYBOOKS_QUERY_KEY } from "../components/PlaybooksSettings/constants";

export default function useCoachSettings() {
    const queryClient = useQueryClient();

    const {
        data: coachSettings,
        isLoading: coachSettingsIsLoading,
        isError: coachSettingsIsError,
        error: coachSettingsError
    } = useQuery({
        queryKey: ['sales-coach-settings'],
        queryFn: getCoachSettings,
    })

    const { mutate: mutateBufferTime } = useMutation({
        mutationKey: ['post-buffer-time'],
        mutationFn: postBufferTime,
        onSuccess: () => {
            queryClient.invalidateQueries({ queryKey: ['sales-coach-settings'] });
        },
        onError: (error) => {
            Sentry.captureException(error);
        }
    })
    
    const { mutate: mutateCueMode } = useMutation({
        mutationKey: ['post-cue-mode'],
        mutationFn: updateCueMode,
        onSuccess: () => {
            queryClient.invalidateQueries({ queryKey: ['sales-coach-settings'] });
        },
        onError: (error) => {
            Sentry.captureException(error);
        }
    })

    const { mutate: mutateAutoStopTimeDelay } = useMutation({
        mutationKey: ['post-auto-stop-time-delay'],
        mutationFn: postAutoStopTimeDelay,
        onSuccess: () => {
            queryClient.invalidateQueries({ queryKey: ['sales-coach-settings'] });
        },
        onError: (error) => {
            Sentry.captureException(error);
        }
    })
    
    const { mutate: mutateUpdateFontSize } = useMutation({
        mutationKey: ['update-font-size'],
        mutationFn: updateFontSize,
        onSuccess: () => {
            queryClient.invalidateQueries({ queryKey: ['sales-coach-settings'] });
        },
        onError: (error) => {
            Sentry.captureException(error);
        }
    })

    const { mutate: mutateOpenLastUsed } = useMutation({
        mutationKey: ['post-open-last-used'],
        mutationFn: postOpenLastUsed,
        onMutate: async (openLastUsed: boolean) => {
            await queryClient.cancelQueries({ queryKey: ['sales-coach-settings'] });
            const prev = queryClient.getQueryData<GetCoachSettingsResponse>(['sales-coach-settings']);
            if (prev) {
                queryClient.setQueryData<GetCoachSettingsResponse>(['sales-coach-settings'], {
                    ...prev,
                    open_last_used: openLastUsed,
                });
            }
            return { prev };
        },
        onError: (error, _val, context) => {
            if (context?.prev !== undefined) {
                queryClient.setQueryData(['sales-coach-settings'], context.prev);
            }
            Sentry.captureException(error);
        },
        onSettled: () => {
            queryClient.invalidateQueries({ queryKey: ['sales-coach-settings'] });
        },
    });

    const { mutate: mutatePulseEnabled } = useMutation({
        mutationKey: ['post-pulse-enabled'],
        mutationFn: postPulseEnabled,
        onMutate: async (pulseEnabled: boolean) => {
            await queryClient.cancelQueries({ queryKey: ['sales-coach-settings'] });
            const prev = queryClient.getQueryData<GetCoachSettingsResponse>(['sales-coach-settings']);
            if (prev) {
                queryClient.setQueryData<GetCoachSettingsResponse>(['sales-coach-settings'], {
                    ...prev,
                    pulse_enabled: pulseEnabled,
                });
            }
            return { prev };
        },
        onError: (error, _val, context) => {
            if (context?.prev !== undefined) {
                queryClient.setQueryData(['sales-coach-settings'], context.prev);
            }
            Sentry.captureException(error);
        },
        onSettled: () => {
            queryClient.invalidateQueries({ queryKey: ['sales-coach-settings'] });
        },
    });

    const { mutate: mutateDefaultPlaybook } = useMutation({
        mutationKey: ['set-default-playbook'],
        mutationFn: setDefaultPlaybook,
        onMutate: async (newId: string) => {
            await queryClient.cancelQueries({ queryKey: ['sales-coach-settings'] });
            await queryClient.cancelQueries({ queryKey: PLAYBOOKS_QUERY_KEY });

            const prevSettings = queryClient.getQueryData<GetCoachSettingsResponse>(['sales-coach-settings']);
            const prevPlaybooks = queryClient.getQueryData<Playbook[]>(PLAYBOOKS_QUERY_KEY);

            if (prevSettings) {
                queryClient.setQueryData<GetCoachSettingsResponse>(['sales-coach-settings'], {
                    ...prevSettings,
                    default_playbook_id: newId,
                });
            }
            if (prevPlaybooks) {
                queryClient.setQueryData<Playbook[]>(
                    PLAYBOOKS_QUERY_KEY,
                    prevPlaybooks.map((p) => ({ ...p, is_default: p.id === newId })),
                );
            }

            return { prevSettings, prevPlaybooks };
        },
        onError: (error, _id, context) => {
            if (context?.prevSettings !== undefined) {
                queryClient.setQueryData(['sales-coach-settings'], context.prevSettings);
            }
            if (context?.prevPlaybooks !== undefined) {
                queryClient.setQueryData(PLAYBOOKS_QUERY_KEY, context.prevPlaybooks);
            }
            Sentry.captureException(error);
        },
        onSettled: () => {
            queryClient.invalidateQueries({ queryKey: ['sales-coach-settings'] });
            queryClient.invalidateQueries({ queryKey: PLAYBOOKS_QUERY_KEY });
        },
    })

    const { mutate: mutatePlaybookOrder } = useMutation({
        mutationKey: ['update-playbook-order'],
        mutationFn: updatePlaybookOrder,
        onMutate: async (order: string[]) => {
            await queryClient.cancelQueries({ queryKey: PLAYBOOKS_QUERY_KEY });
            const prevPlaybooks = queryClient.getQueryData<Playbook[]>(PLAYBOOKS_QUERY_KEY);

            if (prevPlaybooks) {
                const byId = new Map(prevPlaybooks.map((p) => [p.id, p]));
                const reordered = order
                    .map((id) => byId.get(id))
                    .filter((p): p is Playbook => !!p);
                const leftover = prevPlaybooks.filter((p) => !order.includes(p.id));
                queryClient.setQueryData<Playbook[]>(PLAYBOOKS_QUERY_KEY, [...reordered, ...leftover]);
            }

            return { prevPlaybooks };
        },
        onError: (error, _order, context) => {
            if (context?.prevPlaybooks !== undefined) {
                queryClient.setQueryData(PLAYBOOKS_QUERY_KEY, context.prevPlaybooks);
            }
            Sentry.captureException(error);
        },
        onSettled: () => {
            queryClient.invalidateQueries({ queryKey: PLAYBOOKS_QUERY_KEY });
        },
    })

    useEffect(()=>{
        if (coachSettingsError !== null) {
            Sentry.captureException(coachSettingsError)
        }
    },[coachSettingsError])
    
    return {
        coachSettings,
        coachSettingsIsError,
        coachSettingsIsLoading,

        mutateBufferTime,
        mutateCueMode,
        mutateAutoStopTimeDelay,

        mutateUpdateFontSize,
        mutateOpenLastUsed,
        mutatePulseEnabled,
        mutateDefaultPlaybook,
        mutatePlaybookOrder,
    }
}