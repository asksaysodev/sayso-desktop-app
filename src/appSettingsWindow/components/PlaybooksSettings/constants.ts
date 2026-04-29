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

export function getErrorMessage(error: unknown): string {
    if (typeof error === 'object' && error && 'response' in error) {
        const resp = (error as { response?: { data?: { error?: string } } }).response;
        if (resp?.data?.error) return resp.data.error;
    }
    if (error instanceof Error) return error.message;
    return 'Something went wrong.';
}
