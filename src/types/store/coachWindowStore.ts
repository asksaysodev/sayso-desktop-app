import { LeadType } from "@/types/coach";

export type LpmamField = 'location' | 'price' | 'motivation' | 'agent' | 'mortgage' | 'appointment';
export type LpmamData = Record<LpmamField, string | null>;

export type CoachFeature = 'cue';

export type EnabledFeature = 'cue' | 'smart_capture' | 'playbooks' | 'pulse';

export interface SessionData {
    sessionId: string;
    prospectId?: string;
    timestamp?: number;
    session?: unknown;
    enabled_features?: EnabledFeature[];
}

export interface CueInsight {
    id: string;
    message: string;
    createdAt: number;
    priority?: number;
    expiresAt?: number;
    appointmentBooked?: boolean;
    pinned?: boolean;
    pinnedAt?: number;
}

export interface CueState {
    isResettingCueSession: boolean;
    insightsQueue: CueInsight[];
    currentInsight: CueInsight | null;
    isInsightDisplaying: boolean;
    leadType: LeadType | null;
    isInsightsLayoutOpen: boolean;
    hasReceivedFirstInsight: boolean;
    unseenInsightsCount: number;
    lpmama: LpmamData;
    enabledFeatures: EnabledFeature[];
}


export interface CoachWindowStore {
    // ========== SHARED STATE ========== //
    isCoachActive: boolean;
    isCoachLoading: boolean;
    callDurationInSeconds: number;
    sessionData: SessionData | null;
    coachFeature: CoachFeature;
    error: string | null;
    cue: CueState;

    // ========== SHARED ACTIONS ========== //
    setIsCoachActive: (isCoachActive: boolean) => void;
    setIsCoachLoading: (isCoachLoading: boolean) => void;
    setCoachFeature: (coachFeature: CoachFeature) => void;
    clearError: () => void;
    shared_resetCoach: () => void;
    incrementCallDuration: () => void;
    resetCallDuration: () => void;

    // ========== COACH WINDOW ACTIONS ========== //
    openCoachWindow: () => Promise<boolean>;
    closeCoachWindow: () => void;

    // ========== CUE ACTIONS ========== //
    setLeadType: (leadType: LeadType | null) => void;
    cue_createNewSession: (leadType: LeadType | null) => Promise<SessionData>;
    cue_handleStartCue: () => Promise<void>;
    cue_handleStopCue: () => Promise<void>;
    cue_handleLocalTeardown: () => Promise<void>;
    cue_stopSession: (sessionId: string) => Promise<SessionData>;
    cue_resetStates: () => void;
    cue_onPressResetSession: () => Promise<void>;
    cue_removeInsight: (insightId: string) => void;
    cue_addInsight: (insight: Omit<CueInsight, 'createdAt'> & { createdAt?: number }) => void;
    cue_togglePinInsight: (insightId: string) => void;
    cue_showNext: () => void;
    cue_setIsInsightsLayoutOpen: (isInsightsLayoutOpen: boolean) => void;
    cue_setHasReceivedFirstInsight: (hasReceivedFirstInsight: boolean) => void;
    cue_incrementUnseenInsightsCount: () => void;
    cue_resetUnseenInsightsCount: () => void;
    cue_updateSmartCapture: (data: { topic: string; content: string }[]) => void;
}