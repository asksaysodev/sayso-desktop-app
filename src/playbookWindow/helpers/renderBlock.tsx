import { Fragment } from "react/jsx-runtime";
import { PlaybookBlock } from "../types";
import { BLOCK_TEXT_KEY } from "./searchPlaybook";
import formatText, { HighlightRange } from "./formatText";

/** Highlight ranges for one block, keyed by item index (BLOCK_TEXT_KEY for `h2` / `p` text). */
export type BlockHighlights = Map<number, HighlightRange[]>;

export default function renderBlock(block: PlaybookBlock, isFirstItem: boolean = false, highlights?: BlockHighlights) {
    switch (block.type) {
        case 'h2':
            return <Fragment key={block.index}>
                {!isFirstItem && <div className='block-divider' />}
                <h2 className="playbook-block-h2">{formatText(block.text, highlights?.get(BLOCK_TEXT_KEY))}</h2>
            </Fragment>;
        case 'p':
            return <p key={block.index} className="playbook-block-p">{formatText(block.text, highlights?.get(BLOCK_TEXT_KEY))}</p>;
        case 'ul':
            return (
                <ul key={block.index} className="playbook-block-ul">
                    {block.items.map((item, i) => (
                        <li key={i}>{formatText(item, highlights?.get(i))}</li>
                    ))}
                </ul>
            );
        default:
            return null;
    }
}
