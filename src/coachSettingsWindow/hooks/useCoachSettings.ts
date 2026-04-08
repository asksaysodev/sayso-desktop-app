import { useMutation, useQuery, useQueryClient } from "@tanstack/react-query";
import * as Sentry from "@sentry/electron/renderer";
import { useEffect } from "react";
import postBufferTime from "../services/cue/postBufferTime";
import getCoachSettings from "../services/cue/getCoachSettings";
import updateCueMode from "../services/cue/updateCueInsightMode";
import postAutoStopTimeDelay from "../services/cue/postAutoStopTimeDelay";
import updateFontSize from "../services/accessibility/updateFontSize";

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
        
        mutateUpdateFontSize
    }
}