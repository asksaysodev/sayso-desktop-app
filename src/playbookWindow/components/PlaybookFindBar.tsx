import { useEffect, useRef } from 'react';
import type { MouseEvent } from 'react';
import { ChevronLeft, ChevronRight, X } from 'lucide-react';

interface Props {
    query: string;
    matchCount: number;
    activeMatchIndex: number;
    /** Bumped every time Cmd+F is pressed, so an already-open bar re-focuses its input. */
    focusToken: number;
    activeSuggestionId?: string;
    onQueryChange: (query: string) => void;
    onNext: () => void;
    onPrevious: () => void;
    onClose: () => void;
}

const keepFocus = (e: MouseEvent) => e.preventDefault();

export default function PlaybookFindBar({
    query,
    matchCount,
    activeMatchIndex,
    focusToken,
    activeSuggestionId,
    onQueryChange,
    onNext,
    onPrevious,
    onClose,
}: Props) {
    const inputRef = useRef<HTMLInputElement | null>(null);

    useEffect(() => {
        inputRef.current?.focus();
        inputRef.current?.select();
    }, [focusToken]);

    const hasMatches = matchCount > 0;

    return (
        <div className="playbook-find-bar">
            <input
                ref={inputRef}
                type="text"
                className="playbook-find-input"
                placeholder="Find in playbook"
                value={query}
                spellCheck={false}
                aria-label="Find in playbook"
                aria-activedescendant={activeSuggestionId}
                onChange={(e) => onQueryChange(e.target.value)}
            />
            {query.length > 0 && (
                <span className={`playbook-find-counter ${hasMatches ? '' : 'is-empty'}`}>
                    {hasMatches ? `${activeMatchIndex + 1}/${matchCount}` : '0/0'}
                </span>
            )}
            <div className="playbook-find-actions">
                {hasMatches && (
                    <>
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
                    </>
                )}
                <button
                    type="button"
                    className="playbook-find-button"
                    aria-label="Close find bar"
                    onMouseDown={keepFocus}
                    onClick={onClose}
                >
                    <X size={14} />
                </button>
            </div>
        </div>
    );
}
