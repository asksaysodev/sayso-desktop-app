import { Fragment } from "react/jsx-runtime";
import { PlaybookBlock } from "../types";
import formatText from "./formatText";

export default function renderBlock(block: PlaybookBlock, isFirstItem: boolean = false) {
    switch (block.type) {
        case 'h2':
            return <Fragment key={block.index}>
                {!isFirstItem && <div className='block-divider' />}
                <h2 className="playbook-block-h2">{formatText(block.text)}</h2>
            </Fragment>;
        case 'p':
            return <p key={block.index} className="playbook-block-p">{formatText(block.text)}</p>;
        case 'ul':
            return (
                <ul key={block.index} className="playbook-block-ul">
                    {block.items.map((item, i) => (
                        <li key={i}>{formatText(item)}</li>
                    ))}
                </ul>
            );
    }
}