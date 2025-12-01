import { CUE_CONFIG, CUE_PRIORITY_ORDER } from "../../store/coachWindowStore";

/** For handling cases where the toast has expiresAt property and is expired */
export const cue_removeExpired = (queue) => {
    const now = Date.now();
    return queue.filter(insight => insight.expiresAt && insight.expiresAt > now);
};

/** Handle cases where the toast has lived longer than maxAgeBeforeDisplay - meaning it should not be displayed */
export const cue_removeTooOld = (queue) => {
    const now = Date.now();
    return queue.filter(insight => {
        const age = now - insight.createdAt;
        return age <= CUE_CONFIG.maxAgeBeforeDisplay;
    });
};

export const cue_sortByPriority = (queue) => {
    return [...queue].sort((a, b) => CUE_PRIORITY_ORDER[b.priority] - CUE_PRIORITY_ORDER[a.priority]);
};