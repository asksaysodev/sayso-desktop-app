import { ChevronDown, Minus } from 'lucide-react';
import { Playbook } from '@/playbookWindow/types';
import { IS_MAC } from '@/utils/platform';

interface Props {
    selectedPlaybook: Playbook | null;
    isDropdownOpen: boolean;
    showFindHint: boolean;
    onToggleDropdown: () => void;
    onClose: () => void;
}

const FIND_HINT = IS_MAC ? '⌘F' : 'Ctrl+F';

export default function PlaybookHeader({ selectedPlaybook, isDropdownOpen, showFindHint, onToggleDropdown, onClose }: Props) {
    const title = selectedPlaybook ? (selectedPlaybook.alias || selectedPlaybook.file_name) : 'Playbooks';

    return (
        <div className="playbook-header">
            <button
                type="button"
                className={`playbook-title-trigger ${isDropdownOpen ? 'open' : ''}`}
                onClick={onToggleDropdown}
            >
                <span className="playbook-title-text">{title}</span>
                <ChevronDown size={16} className={`playbook-title-chevron ${isDropdownOpen ? 'open' : ''}`} />
            </button>
            <div className="playbook-header-actions">
                {showFindHint && <span className="playbook-find-hint">{FIND_HINT}</span>}
                <button
                    type="button"
                    className="playbook-close-button"
                    onClick={onClose}
                    aria-label="Close playbooks window"
                >
                    <Minus size={16} />
                </button>
            </div>
        </div>
    );
}
