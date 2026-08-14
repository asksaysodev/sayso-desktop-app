import { PlaybookBlock } from '../types';

export function sortBlocks(blocks: PlaybookBlock[] | null | undefined): PlaybookBlock[] {
    if (!blocks) return [];
    return [...blocks].sort((a, b) => a.index - b.index);
}
