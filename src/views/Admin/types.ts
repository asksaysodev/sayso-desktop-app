export type Tool = 'cue-signals' | 'cue-main-instructions';
export type SignalLeadType = 'buyer' | 'seller' | 'all';
export type Signal = {
    id: string;
    lead_type: SignalLeadType;
    priority: number;
    name: string;
    description: string;
    instructions: string;
    stage_fit: {
        connect: string,
        discover: string,
        convert: string,
    } | null;
    stage_instructions: {
        connect: string,
        discover: string,
        convert: string,
    } | null;
};

export interface SignalVersion {
    version: number;
    signals: Signal[];
};

export interface GetSignalResponse {
    activeVersion: number;
    versions: SignalVersion[];
};

export interface UploadSignalSheetResponse {
    version: number;
    message: string;
    uploadedSignals: Signal[];
};
