import { useMutation, useQuery, useQueryClient } from "@tanstack/react-query";
import * as Sentry from "@sentry/electron/renderer";
import { useEffect } from "react";
import postBufferTime from "../services/cue/postBufferTime";
import getCoachSettings from "../services/cue/getCoachSettings";

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

    useEffect(()=>{
        if (coachSettingsError !== null) {
            Sentry.captureException(coachSettingsError)
        }
    },[coachSettingsError])
    
    return {
        coachSettings,
        coachSettingsIsError,
        coachSettingsIsLoading,
        mutateBufferTime
    }
}