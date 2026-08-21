import { useEffect, useRef } from 'react';
import type { MouseEvent } from 'react';
import { ChevronDown, ChevronLeft, ChevronRight, Minus, Search, X } from 'lucide-react';
import { Playbook } from '@/playbookWindow/types';
import { IS_MAC } from '@/utils/platform';

interface Props {
    selectedPlaybook: Playbook | null;
    isDropdownOpen: boolean;
    hasContent: boolean;
    query: string;
    matchCount: number;
    activeMatchIndex: number;
    hasMatches: boolean;
    /** Bumped on Cmd+F, so the input focuses/re-focuses even when already mounted. */
    focusToken: number;
    onToggleDropdown: () => void;
    onQueryChange: (query: string) => void;
    onQueryFocus: () => void;
    onNext: () => void;
    onPrevious: () => void;
    onClearQuery: () => void;
    onClose: () => void;
}

const FIND_HINT = IS_MAC ? '⌘F' : 'Ctrl+F';

// Keeps focus in the search input when a nav/close button is clicked with the mouse.
const keepFocus = (e: MouseEvent) => e.preventDefault();

export default function PlaybookHeader({
    selectedPlaybook,
    isDropdownOpen,
    hasContent,
    query,
    matchCount,
    activeMatchIndex,
    hasMatches,
    focusToken,
    onToggleDropdown,
    onQueryChange,
    onQueryFocus,
    onNext,
    onPrevious,
    onClearQuery,
    onClose,
}: Props) {
    const title = selectedPlaybook ? (selectedPlaybook.alias || selectedPlaybook.file_name) : 'Playbooks';
    const inputRef = useRef<HTMLInputElement | null>(null);

    // Skip the initial token (0): the input is always mounted now, so a mount-time focus
    // would steal keyboard focus the moment the playbook window opens.
    useEffect(() => {
        if (focusToken === 0) return;
        inputRef.current?.focus();
        inputRef.current?.select();
    }, [focusToken]);

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
                {hasContent && (
                    <div className="playbook-search">
                        <div className="playbook-search-field">
                            <Search size={13} className="playbook-search-icon" />
                            <input
                                ref={inputRef}
                                type="text"
                                className="playbook-search-input"
                                placeholder={`Search in Playbooks (${FIND_HINT})`}
                                value={query}
                                spellCheck={false}
                                aria-label="Search in playbooks"
                                onFocus={onQueryFocus}
                                onChange={(e) => onQueryChange(e.target.value)}
                            />
                        </div>
                        {query.length > 0 && (
                            <span className={`playbook-find-counter ${hasMatches ? '' : 'is-empty'}`}>
                                {hasMatches ? `${activeMatchIndex + 1}/${matchCount}` : '0/0'}
                            </span>
                        )}
                        {hasMatches && (
                            <div className="playbook-find-actions">
                                <button
                                    type="button"
                                    className="playbook-find-button"
                                    aria-label="Previous match"
                                    onMouseDown={keepFocus}
                                    onClick={onPrevious}
                                >
                                    <ChevronLeft size={14} />
                                </button>
                                <button
                                    type="button"
                                    className="playbook-find-button"
                                    aria-label="Next match"
                                    onMouseDown={keepFocus}
                                    onClick={onNext}
                                >
                                    <ChevronRight size={14} />
                                </button>
                            </div>
                        )}
                        {query.length > 0 && (
                            <button
                                type="button"
                                className="playbook-find-button playbook-find-clear"
                                aria-label="Clear search"
                                onMouseDown={keepFocus}
                                onClick={() => {
                                    onClearQuery();
                                    // keepFocus only holds focus that was already in the
                                    // input; clicking from the body would otherwise clear
                                    // the query and leave nowhere to type.
                                    inputRef.current?.focus();
                                }}
                            >
                                <X size={14} />
                            </button>
                        )}
                    </div>
                )}
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
