import { CUE_CONFIG } from "../../store/coachWindowStore";
import { CueInsight } from "@/types/store/coachWindowStore";

/** For handling cases where the toast has expiresAt property and is expired. Pinned insights never expire. */
export const cue_removeExpired = (queue: CueInsight[]): CueInsight[] => {
    const now = Date.now();
    return queue.filter(insight => insight.pinned || (insight.expiresAt && insight.expiresAt > now));
};

/** Handle cases where the toast has lived longer than maxAgeBeforeDisplay. Pinned insights are immune. */
export const cue_removeTooOld = (queue: CueInsight[]): CueInsight[] => {
    const now = Date.now();
    return queue.filter(insight => {
        if (insight.pinned) return true;
        const age = now - (insight.createdAt ?? 0);
        return age <= CUE_CONFIG.maxAgeBeforeDisplay;
    });
};

export const cue_sortByPriority = (queue: CueInsight[]): CueInsight[] => {
    return [...queue].sort((a, b) => (a.priority ?? 0) - (b.priority ?? 0));
};
