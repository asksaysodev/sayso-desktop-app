import { LpmamData, LpmamField } from '@/types/store/coachWindowStore';

const LPMAMA_LABELS: { field: LpmamField; label: string }[] = [
    { field: 'location',    label: 'Location' },
    { field: 'price',       label: 'Price' },
    { field: 'motivation',  label: 'Motivation' },
    { field: 'agent',       label: 'Agent' },
    { field: 'mortgage',    label: 'Mortgage' },
    { field: 'appointment', label: 'Appointment' },
];

export const hasCapturedLpmamaData = (lpmama: LpmamData): boolean =>
    Object.values(lpmama).some(v => v !== null);

export const copyLpmamaContent = async (lpmama: LpmamData): Promise<void> => {
    const textToCopy = LPMAMA_LABELS
        .map(({ field, label }) => `${label}: ${lpmama[field] ?? ''}`)
        .join('\n');
    try {
        await navigator.clipboard.writeText(textToCopy);
    } catch {
        // clipboard write failed — silently ignore
    }
};
