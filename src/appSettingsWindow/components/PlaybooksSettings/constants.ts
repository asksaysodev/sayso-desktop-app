import { PlaybookStatus } from '@/playbookWindow/types';

export const PLAYBOOKS_QUERY_KEY = ['playbooks'] as const;
export const PLAYBOOK_LIMIT = 10;
export const MAX_FILE_SIZE_BYTES = 20 * 1024 * 1024;
export const ALLOWED_EXTENSIONS = ['pdf', 'docx', 'txt'] as const;

export const STATUS_LABELS: Record<PlaybookStatus, string> = {
    ready: 'Ready',
    processing: 'Processing',
    failed: 'Failed',
};

export function getFileExtension(name: string): string {
    const idx = name.lastIndexOf('.');
    if (idx < 0) return '';
    return name.slice(idx + 1).toLowerCase();
}

export function validateFile(file: File): string | null {
    const ext = getFileExtension(file.name);
    if (!ALLOWED_EXTENSIONS.includes(ext as (typeof ALLOWED_EXTENSIONS)[number])) {
        return 'Unsupported file type. Use PDF, DOCX, or TXT.';
    }
    if (file.size > MAX_FILE_SIZE_BYTES) {
        return 'File too large. Max 20 MB.';
    }
    return null;
}

export function getUpdateAliasErrorMessage(error: unknown): string {
    const status = (error as { response?: { status?: number } })?.response?.status;
    switch (status) {
        case 400: return 'Please enter a valid alias.';
        case 403: return "You don't have permission to edit this playbook.";
        case 404: return 'This playbook no longer exists. Try refreshing the list.';
        default:  return "Couldn't update the playbook. Please try again.";
    }
}

export function getErrorMessage(error: unknown): string {
    if (typeof error === 'object' && error && 'response' in error) {
        const resp = (error as { response?: { data?: { error?: string } } }).response;
        if (resp?.data?.error) return resp.data.error;
    }
    if (error instanceof Error) return error.message;
    return 'Something went wrong.';
}
