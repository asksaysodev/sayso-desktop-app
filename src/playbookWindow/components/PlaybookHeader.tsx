import { ChevronDown, Minus } from 'lucide-react';
import { Playbook } from '@/playbookWindow/types';

interface Props {
    selectedPlaybook: Playbook | null;
    isDropdownOpen: boolean;
    onToggleDropdown: () => void;
    onClose: () => void;
}

export default function PlaybookHeader({ selectedPlaybook, isDropdownOpen, onToggleDropdown, onClose }: Props) {
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
            <button
                type="button"
                className="playbook-close-button"
                onClick={onClose}
                aria-label="Close playbooks window"
            >
                <Minus size={16} />
            </button>
        </div>
    );
}
