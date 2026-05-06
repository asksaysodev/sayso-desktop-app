import { useEffect, useRef, useState } from 'react';

const STEP_CHANNELS = [
    'onboarding:tray-clicked',
    'onboarding:coach-opened',
    'onboarding:session-started',
    'onboarding:session-stopped',
] as const;

export function useOnboardingProgress(onUnlocked: (step: number) => void) {
    const [unlockedSteps, setUnlockedSteps] = useState<Set<number>>(new Set());
    const onUnlockedRef = useRef(onUnlocked);

    useEffect(() => { onUnlockedRef.current = onUnlocked; }, [onUnlocked]);

    useEffect(() => {
        const ipc = window.electron?.ipcRenderer;
        if (!ipc) return;

        const cleanups = STEP_CHANNELS.map((channel, step) =>
            ipc.on(channel, () => {
                setUnlockedSteps(prev => {
                    if (prev.has(step)) return prev;
                    const next = new Set(prev);
                    next.add(step);
                    return next;
                });
                onUnlockedRef.current(step);
            })
        );

        return () => cleanups.forEach(fn => fn?.());
    }, []);

    return { isUnlocked: (step: number) => unlockedSteps.has(step) };
}
