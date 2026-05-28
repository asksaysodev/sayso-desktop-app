export type UpdatePhase = 'idle' | 'checking' | 'available' | 'downloading' | 'downloaded' | 'error';

export interface UpdateState {
    phase: UpdatePhase;
    currentVersion: string;
    newVersion: string | null;
    progressPercent: number;
    errorMessage: string | null;
}
