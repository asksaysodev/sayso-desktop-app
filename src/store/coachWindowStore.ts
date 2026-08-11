import { create } from 'zustand';
import { v4 } from 'uuid';
import { cue_startStreaming, cue_stopStreaming } from '../coachWindow/services/cueService';
import { cue_removeExpired, cue_removeTooOld, cue_sortByPriority } from '../coachWindow/helpers/cueQueueHelpers';
import apiClient from '../config/axios';
import { CoachFeature, CoachWindowStore, EnabledFeature, LpmamData } from '@/types/store/coachWindowStore';
import { Prospect } from '@/types/coach';

export const CUE_CONFIG = {
    /** Display duration of the toast */
    displayDuration: 6000,
    /** Time between toasts - allows exit animation (300ms) to complete with buffer */
    transitionDelay: 500,
    /** Time until the toast expires */
    expirationTime: 30000,
    /** Animation duration of the toast */
    animationDuration: 300,
    /** Time until the toast is considered too old to display */
    maxAgeBeforeDisplay: 90000, // 90s
    /** Maximum number of insights that can be pinned simultaneously */
    maxPinnedInsights: 3,
};

const LPMAMA_INITIAL: LpmamData = {
    location: null, price: null, motivation: null,
    appointment: null, mortgage: null, agent: null,
};

const CUE_INITIAL_STATE = {
    isResettingCueSession: false,
    insightsQueue: [],
    currentInsight: null,
    isInsightDisplaying: false,
    leadType: null, // 'buyer' or 'seller'
    isInsightsLayoutOpen: false,
    hasReceivedFirstInsight: false,
    unseenInsightsCount: 0,
    lpmama: { ...LPMAMA_INITIAL },
    enabledFeatures: ['cue'] as EnabledFeature[],
};

const getInitialSharedState = () => ({
    isCoachActive: false,
    isCoachLoading: false,
    callDurationInSeconds: 0,
    sessionData: null,
    error: null,
})

let openWindowCheckInterval: NodeJS.Timeout | null = null;
let openWindowTimeoutId: NodeJS.Timeout | null = null;

export const useCoachWindowStore = create<CoachWindowStore>((set, get) => ({
    // ========== SHARED STATE ========== //
    isCoachActive: false,
    isCoachLoading: false,
    callDurationInSeconds: 0,
    sessionData: null,
    coachFeature: 'cue',
    error: null,

    cue: {...CUE_INITIAL_STATE},

    // ========== SHARED ACTIONS ========== //
    setIsCoachActive: (isCoachActive: boolean) => set({ isCoachActive }),
    setIsCoachLoading: (isCoachLoading: boolean) => set({ isCoachLoading }),
    setCoachFeature: (coachFeature: CoachFeature) => set({ coachFeature }),
    clearError: (): void => set({ error: null }),

    // ========== COACH WINDOW ACTIONS ========== //
    openCoachWindow: async () => {
        const cachedPermissions = localStorage.getItem('coachPermissionsGranted') === 'true';

        // If permissions are cached, revalidate to detect OS-level revocations
        if (cachedPermissions) {
            try {
                // Re-check permissions to detect if user revoked them in macOS settings
                const permissionsStatus = await window.electron?.permissions?.check();

                // If microphone permission was revoked, clear cache and block open
                if (!permissionsStatus?.mic) {
                    console.log('[CoachWindowStore] Permissions revoked in OS settings, clearing cache');
                    localStorage.removeItem('coachPermissionsGranted');
                    return false;
                }

                // Mic is still granted - proceed to open window.
                // Screen permission is enforced separately by the start-cue guard in main
                // (and is checked reliably via CGPreflight in permissions.check()).
            } catch (error) {
                console.error('[CoachWindowStore] Error checking permissions:', error);
                // On error, clear cache and block open to be safe
                localStorage.removeItem('coachPermissionsGranted');
                return false;
            }
        } else {
            // No cached permissions — block open
            return false;
        }

        return new Promise((resolve, reject) => {
            const tryOpen = () => {
                if (window.electron?.ipcRenderer) {
                    window.electron.ipcRenderer.send('open-coach-window');
                    return true;
                }
                return false;
            };

            if (tryOpen()) {
                resolve(true);
                return;
            }

            console.log('[CoachWindowStore] Electron not ready, retrying...');
            if (openWindowCheckInterval) clearInterval(openWindowCheckInterval);
            if (openWindowTimeoutId) clearTimeout(openWindowTimeoutId);

            openWindowCheckInterval = setInterval(() => {
                if (tryOpen()) {
                    if (openWindowCheckInterval) clearInterval(openWindowCheckInterval);
                    if (openWindowTimeoutId) clearTimeout(openWindowTimeoutId);
                    console.log('[CoachWindowStore] Coach window opened');
                    resolve(true);
                }
            }, 50);

            openWindowTimeoutId = setTimeout(() => {
                if (openWindowCheckInterval) clearInterval(openWindowCheckInterval);
                if (!window.electron?.ipcRenderer) {
                    console.error('[CoachWindowStore] Electron unavailable after timeout');
                    reject(new Error('Electron not available - timeout after 1s'));
                }
            }, 1000);
        });
    },

    closeCoachWindow: () => {
        if (openWindowCheckInterval) clearInterval(openWindowCheckInterval);
        if (openWindowTimeoutId) clearTimeout(openWindowTimeoutId);

        get().shared_resetCoach();

        if (window.electron && window.electron.ipcRenderer) {
            window.electron.ipcRenderer.send('close-coach-window');
        } else {
            console.warn('[CoachWindowStore] Electron not available, cannot close coach window');
        }
    },

    shared_resetCoach: () => set({
        ...getInitialSharedState(),
    }),

    incrementCallDuration: () => {
        set((state) => ({
            callDurationInSeconds: state.callDurationInSeconds + 1
        }));
    },

    resetCallDuration: () => set({ callDurationInSeconds: 0 }),

    // ========== CUE ACTIONS ========== //
    setLeadType: (leadType) => set((state) => ({
        cue: { ...state.cue, leadType }
    })),

    cue_createNewSession: async (leadType) => {
        try {
            const payload = {
                data: {
                    leadType
                }
            };
            const response = await apiClient.post('/cue/session/new', payload);
            return response.data;
        } catch (error: any) {
            const msg = error.response?.data?.error ?? error.message ?? 'Failed to start session. Please try again.';
			set({ error: msg });
			throw error;
        }
    },

    cue_handleStartCue: async () => {
        set({ isCoachLoading: true });

        try {
            const currentLeadType = get().cue.leadType;

            if (get().isCoachActive) {
                throw new Error('Cue is already active');
            }
            if (!currentLeadType) {
                throw new Error('Scenario is required');
            }
            if (currentLeadType !== 'buyer' && currentLeadType !== 'seller') {
                throw new Error('Scenario must be either "buyer" or "seller"');
            }

            const sessionData = await get().cue_createNewSession(currentLeadType);
            
            if (!sessionData || !sessionData.sessionId) {
                throw new Error('Failed to create new cue session');
            }

            await cue_startStreaming(sessionData.sessionId);

            set({
                sessionData,
                isCoachActive: true,
                cue: {
                    ...get().cue,
                    enabledFeatures: sessionData?.enabled_features ?? ['cue'],
                },
            });
        } catch (error) {
            console.error('Error starting cue:', error);
            throw error;
        } finally {
            set({ isCoachLoading: false })
        }
    },

    cue_handleStopCue: async () => {
        set({ isCoachLoading: true, isCoachActive: false });

        try {
            const sessionId = get().sessionData?.sessionId;
            if(!sessionId) {
                get().cue_resetStates();
                return;
            }

			await cue_stopStreaming();
			const sessionData = await get().cue_stopSession(sessionId);
			if(!sessionData || !sessionData.session) {
				throw new Error('Failed to stop cue session');
			}
            set({ sessionData, isCoachActive: false });

            get().cue_resetStates();

        } catch (error) {
            console.error('Error stopping cue:', error);
            get().cue_resetStates();
            throw error;
        } finally {
			set({ isCoachLoading: false })
        }
    },

    /**
     * The local half of cue_handleStopCue — releases the capture stack and resets
     * state without the POST /cue/session/stop call. For paths where the session is
     * already being persisted elsewhere, or where the credentials are gone and the
     * request could only 401: main persists on logout via stopAndPersistCueSession()
     * (electron/main.ts), so the renderer only has to let go of its own resources.
     *
     * Kept here rather than inlined at the call site so there is one copy of the
     * teardown sequence to keep in step with cue_handleStopCue. Rethrows like
     * cue_handleStopCue does; callers report via reportCoachError.
     */
    cue_handleLocalTeardown: async () => {
        if(!get().isCoachActive) return;

        set({ isCoachActive: false, isCoachLoading: false });
        try {
            await cue_stopStreaming();
        } finally {
            get().cue_resetStates();
        }
    },

    cue_stopSession: async (sessionId) => {

        try {
            const response = await apiClient.post(`/cue/session/stop/${sessionId}`);
            return response.data;
        } catch (error) {
            console.error('Error stopping cue session:', error);
            throw error;
        }
    },

    cue_resetStates: () => {
        set({
            callDurationInSeconds: 0,
            sessionData: null,
            cue: {
                ...CUE_INITIAL_STATE,
                leadType: get().cue.leadType,
            }
        })
    },

    cue_onPressResetSession: async () => {
        if (get().cue.isResettingCueSession) return;

        set({ cue: { ...get().cue, isResettingCueSession: true, isInsightsLayoutOpen: false, enabledFeatures: ['cue'] as EnabledFeature[] } });

        try {
            const currentLeadType = get().cue.leadType;

            const sessionId = get().sessionData?.sessionId;
            const currentEnabledFeatures = get().sessionData?.enabled_features;
            
            if (sessionId) {
                await cue_stopStreaming();
                await get().cue_stopSession(sessionId);
            }

            set({
                callDurationInSeconds: 0,
                sessionData: null,
                cue: {
                    ...CUE_INITIAL_STATE,
                    isResettingCueSession: true,
                    leadType: currentLeadType,
                    enabledFeatures: currentEnabledFeatures ?? ['cue']
                }
            })

            const newSessionData = await get().cue_createNewSession(currentLeadType);

            if (!newSessionData || !newSessionData.sessionId) {
                throw new Error('Failed to create new cue session');
            }

            await cue_startStreaming(newSessionData.sessionId);

            set({
                sessionData: newSessionData,
                isCoachActive: true,
                cue: {
                    ...get().cue,
                    enabledFeatures: newSessionData?.enabled_features ?? ['cue'],
                }
            });
        } catch (error) {
            console.error('Error resetting cue session:', error);
            set({ isCoachActive: false });
            throw error;
        } finally {
            set({
                cue: {
                    ...get().cue,
                    isResettingCueSession: false,
                }
            });
        }
    },

    cue_removeInsight: (insightId) => {
        const prevQueue = get().cue.insightsQueue;
        const newQueue = prevQueue.filter(insight => insight.id !== insightId);
        set((state) => ({
            cue: {
                ...state.cue,
                insightsQueue: newQueue,
                ...(newQueue.length === 0 ? { isInsightsLayoutOpen: false } : {})
            }
        }));
    },

    cue_togglePinInsight: (insightId) => {
        const prevQueue = get().cue.insightsQueue;
        const insight = prevQueue.find(i => i.id === insightId);
        if (!insight) return;

        let newQueue;
        if (insight.pinned) {
            newQueue = prevQueue.map(i =>
                i.id === insightId ? { ...i, pinned: false, pinnedAt: undefined } : i
            );
        } else {
            const pinnedCount = prevQueue.filter(i => i.pinned).length;
            if (pinnedCount >= CUE_CONFIG.maxPinnedInsights) return;
            newQueue = prevQueue.map(i =>
                i.id === insightId ? { ...i, pinned: true, pinnedAt: Date.now() } : i
            );
        }

        set((state) => ({ cue: { ...state.cue, insightsQueue: newQueue } }));
    },

    cue_addInsight: (insight) => {
        const newInsight = {
            ...insight,
            createdAt: insight.createdAt || Date.now(),
        };
        const prevQueue = get().cue.insightsQueue;
        const newQueue = [newInsight, ...prevQueue];

        set((state) => ({
            cue: { ...state.cue, insightsQueue: newQueue }
        }));
    },

    cue_showNext: () => {
        const prevQueue = get().cue.insightsQueue;
        const nextInsight = prevQueue.length > 0 ? prevQueue[0] : null;

        set((state) => ({
            cue: {
                ...state.cue,
                currentInsight: nextInsight,
                isInsightDisplaying: nextInsight !== null,
                insightsQueue: nextInsight ? prevQueue.slice(1) : []
            }
        }));
    },

    cue_setIsInsightsLayoutOpen: (isInsightsLayoutOpen) => {
        set((state) => ({
            cue: { ...state.cue, isInsightsLayoutOpen }
        }));
    },

    cue_setHasReceivedFirstInsight: (hasReceivedFirstInsight) => {
        set((state) => ({
            cue: { ...state.cue, hasReceivedFirstInsight }
        }));
    },

    cue_incrementUnseenInsightsCount: () => {
        set((state) => ({
            cue: { 
                ...state.cue, 
                unseenInsightsCount: state.cue.unseenInsightsCount + 1 
            }
        }));
    },

    cue_resetUnseenInsightsCount: () => {
        set((state) => ({
            cue: { ...state.cue, unseenInsightsCount: 0 }
        }));
    },

    cue_updateSmartCapture: (data) => {
        set((state) => {
            const updatedLpmama = { ...state.cue.lpmama };
            data.forEach(({ topic, content }) => {
                if (topic in updatedLpmama) {
                    (updatedLpmama as Record<string, string | null>)[topic] = content;
                }
            });
            return { cue: { ...state.cue, lpmama: updatedLpmama } };
        });
    },
}));

// Methods for use outside React (e.g. shortcuts feature)
export const openCoachWindow = () => {
    return useCoachWindowStore.getState().openCoachWindow();
};
