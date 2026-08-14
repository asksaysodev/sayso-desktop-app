import { ReactNode } from 'react';
import { TextRange } from './searchPlaybook';

// Unlike formatText this applies no bracket styling, so a playbook named "[Draft] FSBO"
// stays literal. Ranges must be ordered and non-overlapping.
export default function highlightText(text: string, ranges: TextRange[]): ReactNode {
    if (ranges.length === 0) return text;

    const nodes: ReactNode[] = [];
    let cursor = 0;
    for (const range of ranges) {
        if (range.start > cursor) nodes.push(text.slice(cursor, range.start));
        nodes.push(
            <mark key={range.start} className="playbook-find-match">
                {text.slice(range.start, range.end)}
            </mark>,
        );
        cursor = range.end;
    }
    if (cursor < text.length) nodes.push(text.slice(cursor));

    return nodes;
}
