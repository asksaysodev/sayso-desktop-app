export type PlaybookStatus = 'processing' | 'ready' | 'failed';
export type PlaybookType = 'custom' | 'public' | 'purchasable';

export interface PlaybookBlockHeading {
    index: number;
    type: 'h2';
    text: string;
}
export interface PlaybookBlockParagraph {
    index: number;
    type: 'p';
    text: string;
}
export interface PlaybookBlockList {
    index: number;
    type: 'ul';
    items: string[];
}
export type PlaybookBlock = PlaybookBlockHeading | PlaybookBlockParagraph | PlaybookBlockList;

export interface Playbook {
    id: string;
    account_id: string;
    alias: string | null;
    file_name: string;
    file_size: number;
    file_type: string;
    status: PlaybookStatus;
    type: PlaybookType;
    price?: number | null;
    currency?: string | null;
    blocks: PlaybookBlock[] | null;
    purchased?: boolean;
    is_default: boolean;
}

export interface GetPlaybooksResponse {
    playbooks: Playbook[];
}

export interface UploadPlaybookResponse {
    playbook: Playbook;
}

export interface PlaybooksCachePayload {
    playbooks: Playbook[] | null;
    error: string | null;
}
