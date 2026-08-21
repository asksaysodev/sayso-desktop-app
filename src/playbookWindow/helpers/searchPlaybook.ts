import { PlaybookBlock } from '../types';
import { sortBlocks } from './blocks';

export const BLOCK_TEXT_KEY = -1;

export interface PlaybookMatch {
    blockIndex: number;
    /** Index within a `ul` block's items, or BLOCK_TEXT_KEY for `h2` / `p` text. */
    itemIndex: number;
    start: number;
    end: number;
}

interface TextRange {
    start: number;
    end: number;
}

function escapeRegExp(value: string): string {
    return value.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
}

// Matches against the original text rather than a lowercased copy: case folding is
// not length-preserving, so offsets taken from a copy would drift.
function createMatcher(query: string): RegExp | null {
    if (!query) return null;
    return new RegExp(escapeRegExp(query), 'gi');
}

function findRanges(text: string, matcher: RegExp): TextRange[] {
    if (!text) return [];
    matcher.lastIndex = 0;
    const ranges: TextRange[] = [];
    let match: RegExpExecArray | null;
    while ((match = matcher.exec(text)) !== null) {
        ranges.push({ start: match.index, end: match.index + match[0].length });
    }
    return ranges;
}

function blockTexts(block: PlaybookBlock): Array<{ itemIndex: number; text: string }> {
    if (block.type === 'ul') {
        return block.items.map((text, itemIndex) => ({ itemIndex, text }));
    }
    return [{ itemIndex: BLOCK_TEXT_KEY, text: block.text }];
}

function collectMatches(sorted: PlaybookBlock[], matcher: RegExp): PlaybookMatch[] {
    const matches: PlaybookMatch[] = [];
    for (const block of sorted) {
        for (const { itemIndex, text } of blockTexts(block)) {
            for (const { start, end } of findRanges(text, matcher)) {
                matches.push({ blockIndex: block.index, itemIndex, start, end });
            }
        }
    }
    return matches;
}

export function findMatches(blocks: PlaybookBlock[] | null | undefined, query: string): PlaybookMatch[] {
    const matcher = createMatcher(query);
    if (!matcher) return [];
    return collectMatches(sortBlocks(blocks), matcher);
}

export function matchKey(match: PlaybookMatch): string {
    return `${match.blockIndex}:${match.itemIndex}:${match.start}`;
}

/** Keeps the reader near where they were when a query is refined. */
export function indexAtOrAfter(matches: PlaybookMatch[], anchor: PlaybookMatch | null): number {
    if (!anchor) return 0;
    const index = matches.findIndex((match) => {
        if (match.blockIndex !== anchor.blockIndex) return match.blockIndex > anchor.blockIndex;
        if (match.itemIndex !== anchor.itemIndex) return match.itemIndex > anchor.itemIndex;
        return match.start >= anchor.start;
    });
    return index === -1 ? 0 : index;
}
