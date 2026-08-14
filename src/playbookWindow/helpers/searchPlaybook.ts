import { Playbook, PlaybookBlock } from '../types';
import { sortBlocks } from './blocks';

export const BLOCK_TEXT_KEY = -1;

export interface PlaybookMatch {
    blockIndex: number;
    /** Index within a `ul` block's items, or BLOCK_TEXT_KEY for `h2` / `p` text. */
    itemIndex: number;
    start: number;
    end: number;
}

export interface TextRange {
    start: number;
    end: number;
}

export interface SuggestionSnippet {
    text: string;
    start: number;
    end: number;
}

export interface PlaybookSuggestion {
    playbookId: string;
    label: string;
    labelRanges: TextRange[];
    /** 0 for a name-only match. */
    matchCount: number;
    snippet: SuggestionSnippet | null;
}

const SNIPPET_RADIUS = 26;

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

function textForMatch(sorted: PlaybookBlock[], match: PlaybookMatch): string {
    const block = sorted.find((b) => b.index === match.blockIndex);
    if (!block) return '';
    if (block.type === 'ul') return block.items[match.itemIndex] ?? '';
    return block.text;
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

function buildSnippet(text: string, start: number, end: number): SuggestionSnippet {
    const from = Math.max(0, start - SNIPPET_RADIUS);
    const to = Math.min(text.length, end + SNIPPET_RADIUS);
    const prefix = from > 0 ? '…' : '';
    const suffix = to < text.length ? '…' : '';
    const snippetStart = prefix.length + (start - from);

    return {
        text: `${prefix}${text.slice(from, to)}${suffix}`,
        start: snippetStart,
        end: snippetStart + (end - start),
    };
}

/** Other playbooks matching by name, by content, or both — name matches first. */
export function buildSuggestions(
    playbooks: Playbook[] | null,
    excludeId: string | null,
    query: string,
): PlaybookSuggestion[] {
    const matcher = createMatcher(query);
    if (!matcher || !playbooks) return [];

    const suggestions: PlaybookSuggestion[] = [];
    for (const playbook of playbooks) {
        if (playbook.id === excludeId) continue;
        // The selector refuses to open non-ready playbooks, so a row for one is a dead end.
        if (playbook.status !== 'ready') continue;

        // The displayed name only: matching a hidden file_name would surface a
        // suggestion with no visible reason for being there.
        const label = playbook.alias || playbook.file_name;
        const labelRanges = findRanges(label, matcher);

        const sorted = sortBlocks(playbook.blocks);
        const matches = collectMatches(sorted, matcher);
        if (labelRanges.length === 0 && matches.length === 0) continue;

        const first = matches[0];
        suggestions.push({
            playbookId: playbook.id,
            label,
            labelRanges,
            matchCount: matches.length,
            snippet: first ? buildSnippet(textForMatch(sorted, first), first.start, first.end) : null,
        });
    }

    return suggestions.sort((a, b) => {
        const aNamed = a.labelRanges.length > 0 ? 1 : 0;
        const bNamed = b.labelRanges.length > 0 ? 1 : 0;
        if (aNamed !== bNamed) return bNamed - aNamed;
        return b.matchCount - a.matchCount;
    });
}
