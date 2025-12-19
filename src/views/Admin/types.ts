export type Tool = 'cue-signals' | 'cue-main-instructions';
export type Signal = {
    id: string;
    description: string;
    instructions: string;
    lead_type: string;
    name: string;
    priority: number;
    threshold: number;
}