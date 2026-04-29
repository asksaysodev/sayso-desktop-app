import { Playbook } from '@/playbookWindow/types';
import renderBlock from '../helpers/renderBlock';

interface Props {
    playbook: Playbook | null;
}

export default function PlaybookBody({ playbook }: Props) {
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

    if (!playbook.blocks || playbook.blocks.length === 0) {
        return (
            <div className="playbook-body playbook-body-empty">
                <p>This playbook has no content yet.</p>
            </div>
        );
    }

    const sortedBlocks = [...playbook.blocks].sort((a, b) => a.index - b.index);

    return (
        <div className="playbook-body">
            {sortedBlocks.map((block, index) => {
                const isFirstItem = index === 0;
                return (
                    renderBlock(block,isFirstItem)
                )
            })}
        </div>
    );
}