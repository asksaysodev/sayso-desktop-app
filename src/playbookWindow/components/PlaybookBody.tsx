import { useEffect, useMemo, useRef } from 'react';
import { Playbook } from '@/playbookWindow/types';
import renderBlock, { BlockHighlights } from '../helpers/renderBlock';
import { sortBlocks } from '../helpers/blocks';
import { PlaybookMatch, matchKey } from '../helpers/searchPlaybook';

interface Props {
    playbook: Playbook | null;
    matches: PlaybookMatch[];
    activeMatchIndex: number;
}

export default function PlaybookBody({ playbook, matches, activeMatchIndex }: Props) {
    const bodyRef = useRef<HTMLDivElement | null>(null);

    const sortedBlocks = useMemo(() => sortBlocks(playbook?.blocks), [playbook?.blocks]);

    const highlightsByBlock = useMemo(() => {
        const byBlock = new Map<number, BlockHighlights>();
        matches.forEach((match, matchIndex) => {
            let blockHighlights = byBlock.get(match.blockIndex);
            if (!blockHighlights) {
                blockHighlights = new Map();
                byBlock.set(match.blockIndex, blockHighlights);
            }
            const ranges = blockHighlights.get(match.itemIndex) ?? [];
            ranges.push({
                start: match.start,
                end: match.end,
                matchIndex,
                isActive: matchIndex === activeMatchIndex,
            });
            blockHighlights.set(match.itemIndex, ranges);
        });
        return byBlock;
    }, [matches, activeMatchIndex]);

    const activeMatch = matches[activeMatchIndex];
    const activeMatchKey = activeMatch ? `${playbook?.id}:${matchKey(activeMatch)}` : null;

    useEffect(() => {
        if (bodyRef.current) bodyRef.current.scrollTop = 0;
    }, [playbook?.id]);

    useEffect(() => {
        if (!activeMatchKey) return;
        const active = bodyRef.current?.querySelector(`[data-match-index="${activeMatchIndex}"]`);
        active?.scrollIntoView({ block: 'center' });
    }, [activeMatchKey]);

    if (!playbook) {
        return (
            <div className="playbook-body playbook-body-empty">
                <p>Select a playbook to view its content.</p>
            </div>
        );
    }

    if (playbook.status === 'processing') {
        return (
            <div className="playbook-body playbook-body-empty">
                <p>This playbook is still processing. Check back in a moment.</p>
            </div>
        );
    }

    if (playbook.status === 'failed') {
        return (
            <div className="playbook-body playbook-body-empty">
                <p>This playbook failed to process.</p>
            </div>
        );
    }

    if (sortedBlocks.length === 0) {
        return (
            <div className="playbook-body playbook-body-empty">
                <p>This playbook has no content yet.</p>
            </div>
        );
    }

    return (
        <div ref={bodyRef} className="playbook-body">
            {sortedBlocks.map((block, index) =>
                renderBlock(block, index === 0, highlightsByBlock.get(block.index)),
            )}
        </div>
    );
}
