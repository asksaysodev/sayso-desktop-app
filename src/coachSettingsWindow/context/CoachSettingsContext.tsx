import { createContext, useContext } from 'react';
import useCoachSettings from '../hooks/useCoachSettings';
import { GetCoachSettingsResponse, PostBufferTimeResponse } from '../types';
import { UseMutateFunction } from '@tanstack/react-query';

interface CoachSettingsContextValue {
    coachSettings: GetCoachSettingsResponse | undefined;
    coachSettingsIsLoading: boolean;
    mutateBufferTime: UseMutateFunction<PostBufferTimeResponse, Error, string | number, unknown>;
};

const CoachSettingsContext = createContext<CoachSettingsContextValue>({} as CoachSettingsContextValue);

export const CoachSettingsProvider = ({ children }: { children: React.ReactNode }) => {

    const { coachSettings, coachSettingsIsLoading, mutateBufferTime } = useCoachSettings();

    const values = { coachSettings, coachSettingsIsLoading, mutateBufferTime };
    
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