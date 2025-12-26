import { create } from 'zustand';
import { v4 } from 'uuid';
import { uploadFullRecording } from '../coachWindow/services/audioUploadService';
import { getProspects } from '../coachWindow/services/recallService';
import { stopDualChannelRecording, startDualChannelRecording } from '../coachWindow/services/audioRecordingService';
import { cue_startStreaming, cue_stopStreaming } from '../coachWindow/services/cueService';
import { cue_removeExpired, cue_removeTooOld, cue_sortByPriority } from '../coachWindow/helpers/cueQueueHelpers';
import apiClient from '../config/axios';

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
};

const AUDIO_INITIAL_STATE = {
    isCompressing: false,
    isUploading: false,
};

const CUE_INITIAL_STATE = {
    isResettingCueSession: false,
    insightsQueue: [],
    currentInsight: null,
    isInsightDisplaying: false,
    leadType: null, // 'buyer' or 'seller'
    isInsightsLayoutOpen: false,
};

const RECALL_INITIAL_STATE = {
    selectedProspect: null,
    prospects: [],
    isLoadingProspects: false,
    prospectsError: null,
    signals: []
};

const getInitialSharedState = () => ({
    isCoachActive: false,
    isCoachLoading: false,
    callDurationInSeconds: 0,
    sessionData: null,
    error: null,
    audio: { ...AUDIO_INITIAL_STATE },
})

let openWindowCheckInterval = null;
let openWindowTimeoutId = null;

export const useCoachWindowStore = create((set, get) => ({
    // ========== SHARED STATE ========== //
    isCoachActive: false,
    isCoachLoading: false,
    callDurationInSeconds: 0,
    sessionData: null,
    coachFeature: 'cue', // 'cue' or 'recall'
    error: null, // string | null

    audio: {...AUDIO_INITIAL_STATE},

    cue: {...CUE_INITIAL_STATE},

    recall: {...RECALL_INITIAL_STATE},

    // ========== PERMISSIONS STATE ========== //
    showPermissionsModal: false,
    needsSystemSettings: false,

    // ========== SHARED ACTIONS ========== //
    setIsCoachActive: (isCoachActive) => set({ isCoachActive }),
    setIsCoachLoading: (isCoachLoading) => set({ isCoachLoading }),
    setCoachFeature: (coachFeature) => set({ coachFeature }),
    clearError: () => set({ error: null }),

    // ========== PERMISSIONS ACTIONS ========== //
    requestPermissions: async () => {
        try {
            const result = await window.electron.permissions.requestAll();
            if (result?.mic && result?.screen) {
                localStorage.setItem('coachPermissionsGranted', 'true');
                set({ showPermissionsModal: false, needsSystemSettings: false });
                return true;
            }

            set({ needsSystemSettings: true });
            return false;

        } catch (error) {
            console.error('Error requesting permissions:', error);
            set({ needsSystemSettings: true });
            return false;
        }
    },

    closePermissionsModal: () => {
        set({ showPermissionsModal: false, needsSystemSettings: false });
    },

    openCoachWindow: async () => {
        const cachedPermissions = localStorage.getItem('coachPermissionsGranted') === 'true';

        // If permissions are cached, revalidate to detect OS-level revocations
        if (cachedPermissions) {
            try {
                // Re-check permissions to detect if user revoked them in macOS settings
                const permissionsStatus = await window.electron?.permissions?.check();

                // If microphone permission was revoked, clear cache and show modal
                if (!permissionsStatus?.mic) {
                    console.log('[CoachWindowStore] Permissions revoked in OS settings, clearing cache');
                    localStorage.removeItem('coachPermissionsGranted');
                    set({ showPermissionsModal: true, needsSystemSettings: false });
                    return false;
                }

                // Mic is still granted - proceed to open window
                // Note: Screen permission can't be reliably checked without requesting,
                // so we'll handle that when actually trying to use it
            } catch (error) {
                console.error('[CoachWindowStore] Error checking permissions:', error);
                // On error, clear cache and show modal to be safe
                localStorage.removeItem('coachPermissionsGranted');
                set({ showPermissionsModal: true, needsSystemSettings: false });
                return false;
            }
        } else {
            // No cached permissions, show modal
            set({ showPermissionsModal: true, needsSystemSettings: false });
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

            console.log('⏳ [CoachWindowStore] Electron not ready, retrying...');
            clearInterval(openWindowCheckInterval);
            clearTimeout(openWindowTimeoutId);

            openWindowCheckInterval = setInterval(() => {
                if (tryOpen()) {
                    clearInterval(openWindowCheckInterval);
                    clearTimeout(openWindowTimeoutId);
                    console.log('[CoachWindowStore] Coach window opened');
                    resolve(true);
                }
            }, 50);

            openWindowTimeoutId = setTimeout(() => {
                clearInterval(openWindowCheckInterval);
                if (!window.electron?.ipcRenderer) {
                    console.error('[CoachWindowStore] Electron unavailable after timeout');
                    reject(new Error('Electron not available - timeout after 1s'));
                }
            }, 1000);
        });
    },

    closeCoachWindow: () => {
        clearInterval(openWindowCheckInterval);
        clearTimeout(openWindowTimeoutId);

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

    recall_createNewSessionData: (prospectId) => {
        if (!prospectId) return null;

        const newSessionId = v4();
        const newSessionData = {
            sessionId: newSessionId,
            prospectId: prospectId,
            timestamp: Date.now()
        }
        set({ sessionData: newSessionData});
        return newSessionData;
    },

    // ========== RECALL ACTIONS ========== //
    setSelectedProspect: (selectedProspect) => set((state) => ({
        recall: {
            ...state.recall,
            selectedProspect
        }
    })),

    setProspects: (prospects) => set((state) => ({
        recall: {
            ...state.recall,
            prospects
        }
    })),

    recall_fetchProspects: async () => {
        set((state) => ({
            recall: {
                ...state.recall,
                isLoadingProspects: true,
            }
        }));

        try {
            const prospectsData = await getProspects();

            set((state) => ({
                recall: {
                    ...state.recall,
                    prospects: prospectsData,
                    isLoadingProspects: false
                }
            }));
        } catch (error) {
            console.error('Error fetching prospects:', error);
            set((state) => ({
                recall: {
                    ...state.recall,
                    isLoadingProspects: false,
                    prospectsError: error.message
                }
            }));
        }
    },

    recall_startDualChannelRecording: async (prospectId) => {
        set({ isCoachLoading: true })

        try {
            // Create session data to get sessionId and prospectId
            const sessionData = get().recall_createNewSessionData(prospectId);

            if (!sessionData) {
                set({ isCoachLoading: false })
                return;
            }

            const recordingParams = {
                sessionId: sessionData.sessionId,
                prospectId: prospectId.prospectId,
                metadata: {
                    sessionId: sessionData.sessionId,
                    prospectId: sessionData.prospectId,
                    timestamp: sessionData.timestamp
                }
            };

            const result = await startDualChannelRecording(recordingParams);
            set({
                isCoachLoading: false,
                isCoachActive: true
            })
            return result;
        } catch (error) {
            set({ isCoachLoading: false });
            throw error;
        }
    },

    recall_handleStopRecording: async () => {
        try {
            set({
                isCoachLoading: true,
                isCoachActive: false
            });

            const result = await stopDualChannelRecording();
            console.log('[Store] Recording stopped:', result);

            set((state) => ({
                audio: {
                    ...state.audio,
                    isCompressing: true
                }
            }));

            set((state) => ({
                audio: {
                    ...state.audio,
                    isUploading: true,
                    isCompressing: false
                }
            }));
            const uploadResults = await uploadFullRecording(result);
            console.log('[Store] Files uploaded:', uploadResults);

            set((state) => ({
                audio: {
                    ...state.audio,
                    isUploading: false
                }
            }));
            return uploadResults;

        } catch (error) {
            console.error('[Store] Error:', error);
            throw error;
        } finally {
            get().recall_resetCoach();
        }
    },

    setSignals: (signals) => set((state) => ({
        recall: { ...state.recall, signals }
    })),

    recall_resetCoach: () => {
        set((state) => ({
            ...getInitialSharedState(),
            recall: {
                ...RECALL_INITIAL_STATE,
                selectedProspect: state.recall.selectedProspect,
                prospects: state.recall.prospects,
            }
        }));
    },

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
        } catch (error) {
            console.error('Error creating new cue session:', error);
            console.log('123123', error.response?.data.error);
            set({ error: error.response?.data?.error });
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

            set({ sessionData, isCoachActive: true });
        } catch (error) {
            console.error('Error starting cue:', error);
            throw error;
        } finally {
            set({ isCoachLoading: false })
        }
    },

    cue_handleStopCue: async () => {
        set({ isCoachLoading: true });

        try {
            const sessionId = get().sessionData?.sessionId;
            if(!sessionId) {
                throw new Error('Session ID is required');
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
            throw error;
        } finally {
			set({ isCoachLoading: false })
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

        set({ cue: { ...get().cue, isResettingCueSession: true } });

        try {
            const currentLeadType = get().cue.leadType;

            const sessionId = get().sessionData?.sessionId;

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
                }
            })

            const sessionData = await get().cue_createNewSession(currentLeadType);

            if (!sessionData || !sessionData.sessionId) {
                throw new Error('Failed to create new cue session');
            }

            await cue_startStreaming(sessionData.sessionId);

            set({
                sessionData,
                isCoachActive: true
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
            cue: { ...state.cue, insightsQueue: newQueue }
        }));
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
}));

// Methods for use outside React (e.g. shortcuts feature)
export const openCoachWindow = () => {
    return useCoachWindowStore.getState().openCoachWindow();
};
