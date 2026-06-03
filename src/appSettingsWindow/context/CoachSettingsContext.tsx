import { createContext, useContext } from 'react';
import useCoachSettings from '../hooks/useCoachSettings';
import { AccessibilityFontSizeType, CueMode, GetCoachSettingsResponse, PostAutoStopTimeDelayResponse, PostBufferTimeResponse, PostOpenLastUsedResponse } from '../types';
import { UseMutateFunction } from '@tanstack/react-query';

interface CoachSettingsContextValue {
    coachSettings: GetCoachSettingsResponse | undefined;
    coachSettingsIsLoading: boolean;
    mutateBufferTime: UseMutateFunction<PostBufferTimeResponse, Error, string | number, unknown>;
    mutateCueMode: UseMutateFunction<unknown, Error, CueMode, unknown>;
    mutateAutoStopTimeDelay: UseMutateFunction<PostAutoStopTimeDelayResponse, Error, number, unknown>;
    mutateUpdateFontSize: UseMutateFunction<unknown, Error, AccessibilityFontSizeType, unknown>;
    mutateOpenLastUsed: UseMutateFunction<PostOpenLastUsedResponse, Error, boolean, unknown>;
    mutateDefaultPlaybook: UseMutateFunction<{ success: true; default_playbook_id: string }, Error, string, unknown>;
};

const CoachSettingsContext = createContext<CoachSettingsContextValue | null>(null);

export const CoachSettingsProvider = ({ children }: { children: React.ReactNode }) => {
    const {
        coachSettings,
        coachSettingsIsLoading,
        mutateBufferTime,
        mutateCueMode,
        mutateAutoStopTimeDelay,
        mutateUpdateFontSize,
        mutateOpenLastUsed,
        mutateDefaultPlaybook,
    } = useCoachSettings();

    const values = {
        coachSettings,
        coachSettingsIsLoading,
        mutateBufferTime,
        mutateCueMode,
        mutateAutoStopTimeDelay,
        mutateUpdateFontSize,
        mutateOpenLastUsed,
        mutateDefaultPlaybook,
    };
    
    return (
        <CoachSettingsContext.Provider value={values}>
            {children}
        </CoachSettingsContext.Provider>
    )
}

export default function useCoachSettingsContext() {
    const context = useContext(CoachSettingsContext);
    if (!context) {
        throw new Error('useCoachSettingsContext must be used within a CoachSettingsProvider');
    }
    return context;
}