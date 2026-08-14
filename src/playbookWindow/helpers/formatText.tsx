import { ReactNode } from 'react';

export interface HighlightRange {
    start: number;
    end: number;
    /** Position in the playbook-wide match list; emitted as `data-match-index`. */
    matchIndex: number;
    isActive: boolean;
}

interface Range {
    start: number;
    end: number;
}

function bracketRanges(text: string): Range[] {
    const pattern = /\[.*?\]/g;
    const ranges: Range[] = [];
    let match: RegExpExecArray | null;
    while ((match = pattern.exec(text)) !== null) {
        ranges.push({ start: match.index, end: match.index + match[0].length });
    }
    return ranges;
}

function contains(range: Range, start: number, end: number) {
    return range.start <= start && end <= range.end;
}

// Splits text at every bracket and highlight boundary so a segment is wholly inside
// or wholly outside each, then wraps each segment in whichever styling applies.
export default function formatText(text: string, highlights: HighlightRange[] = []) {
    if (!text) return text;

    const brackets = bracketRanges(text);
    if (brackets.length === 0 && highlights.length === 0) return text;

    const boundaries = new Set<number>([0, text.length]);
    for (const range of [...brackets, ...highlights]) {
        boundaries.add(range.start);
        boundaries.add(range.end);
    }
    const points = [...boundaries]
        .filter((point) => point >= 0 && point <= text.length)
        .sort((a, b) => a - b);

    const nodes: ReactNode[] = [];
    for (let i = 0; i < points.length - 1; i++) {
        const start = points[i];
        const end = points[i + 1];
        const segment = text.slice(start, end);
        if (!segment) continue;

        const isBracket = brackets.some((range) => contains(range, start, end));
        const highlight = highlights.find((range) => contains(range, start, end));

        if (highlight) {
            nodes.push(
                <mark
                    key={start}
                    className={`playbook-find-match${highlight.isActive ? ' is-active' : ''}`}
                    data-match-index={highlight.matchIndex}
                >
                    {isBracket ? <span className="playbook-highlight-bracket">{segment}</span> : segment}
                </mark>,
            );
        } else if (isBracket) {
            nodes.push(
                <span key={start} className="playbook-highlight-bracket">
                    {segment}
                </span>,
            );
        } else {
            nodes.push(segment);
        }
    }

    return nodes;
}
