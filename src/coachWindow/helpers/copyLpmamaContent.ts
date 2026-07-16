import { LpmamData } from '@/types/store/coachWindowStore';
import { LPMAMA_CONFIG } from '../constants/lpmama';

export const copyLpmamaContent = async (lpmama: LpmamData): Promise<void> => {
    const textToCopy = LPMAMA_CONFIG
        .map(({ field, label }) => `${label}: ${lpmama[field] ?? ''}`)
        .join('\n');
    try {
        await navigator.clipboard.writeText(textToCopy);
    } catch {
        // clipboard write failed — silently ignore
    }
};
