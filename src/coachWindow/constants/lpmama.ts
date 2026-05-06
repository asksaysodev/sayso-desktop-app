import { LpmamField } from '@/types/store/coachWindowStore';

export const LPMAMA_CONFIG: { field: LpmamField; initial: string; label: string }[] = [
    { field: 'location',    initial: 'L', label: 'Location' },
    { field: 'price',       initial: 'P', label: 'Price' },
    { field: 'motivation',  initial: 'M', label: 'Motivation' },
    { field: 'agent',       initial: 'A', label: 'Agent' },
    { field: 'mortgage',    initial: 'M', label: 'Mortgage' },
    { field: 'appointment', initial: 'A', label: 'Appointment' },
];
