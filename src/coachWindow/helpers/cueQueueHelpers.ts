import { CUE_CONFIG } from "../../store/coachWindowStore";
import { Insight } from "@/types/coach";

/** For handling cases where the toast has expiresAt property and is expired */
export const cue_removeExpired = (queue: Insight[]): Insight[] => {
    const now = Date.now();
    return queue.filter(insight => insight.expiresAt && insight.expiresAt > now);
};

/** Handle cases where the toast has lived longer than maxAgeBeforeDisplay - meaning it should not be displayed */
export const cue_removeTooOld = (queue: Insight[]): Insight[] => {
    const now = Date.now();
    return queue.filter(insight => {
        const age = now - (insight.createdAt ?? 0);
        return age <= CUE_CONFIG.maxAgeBeforeDisplay;
    });
};

export const cue_sortByPriority = (queue: Insight[]): Insight[] => {
    return [...queue].sort((a, b) => (a.priority ?? 0) - (b.priority ?? 0));
};
