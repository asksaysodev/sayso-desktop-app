import { useEffect, useMemo, useRef, useState } from 'react';
import { Spinner } from '@/components/ui/spinner';
import { usePlaybooksCache } from './hooks/usePlaybooksCache';
import { usePlaybookPrefetch } from './hooks/usePlaybookPrefetch';
import { useOpenLastUsedSetting } from './hooks/useOpenLastUsedSetting';
import {
    PlaybookMatch,
    PlaybookSuggestion,
    buildSuggestions,
    findMatches,
    indexAtOrAfter,
} from './helpers/searchPlaybook';
import PlaybookHeader from './components/PlaybookHeader';
import PlaybookSelector from './components/PlaybookSelector';
import PlaybookBody from './components/PlaybookBody';
import PlaybookFindBar from './components/PlaybookFindBar';
import FindSuggestions from './components/FindSuggestions';

const LAST_USED_KEY = 'sayso:lastUsedPlaybookId';

export default function PlaybookWindowApp() {
    usePlaybookPrefetch();
    const { playbooks: rawPlaybooks, error, isLoading } = usePlaybooksCache();
    const { openLastUsed, loaded: settingLoaded } = useOpenLastUsedSetting();
    const [selectedId, setSelectedId] = useState<string | null>(null);
    const [isDropdownOpen, setIsDropdownOpen] = useState(false);
    const dropdownRef = useRef<HTMLDivElement | null>(null);
    const [isFindOpen, setIsFindOpen] = useState(false);
    const [query, setQuery] = useState('');
    const [activeMatchIndex, setActiveMatchIndex] = useState(0);
    const [activeSuggestionIndex, setActiveSuggestionIndex] = useState(0);
    const [findFocusToken, setFindFocusToken] = useState(0);
    // Document position of the last active match, so narrowing a query resumes nearby.
    const matchAnchorRef = useRef<PlaybookMatch | null>(null);

    const playbooks = useMemo(() => {
        if (!rawPlaybooks) return rawPlaybooks;
        return [...rawPlaybooks].sort((a, b) => Number(b.is_default) - Number(a.is_default));
    }, [rawPlaybooks]);

    useEffect(() => {
        if (!settingLoaded) return;
        if (!playbooks || playbooks.length === 0) {
            if (selectedId !== null) setSelectedId(null);
            return;
        }
        const stillExists = selectedId && playbooks.some((p) => p.id === selectedId);
        if (stillExists) return;

        if (openLastUsed) {
            const lastUsedId = localStorage.getItem(LAST_USED_KEY);
            const lastUsed = lastUsedId ? playbooks.find((p) => p.id === lastUsedId && p.status === 'ready') : null;
            if (lastUsed) {
                setSelectedId(lastUsed.id);
                return;
            }
        }

        const defaultReady = playbooks.find((p) => p.is_default && p.status === 'ready');
        const firstReady = playbooks.find((p) => p.status === 'ready');
        setSelectedId((defaultReady ?? firstReady ?? playbooks[0]).id);
    }, [playbooks, selectedId, openLastUsed, settingLoaded]);

    useEffect(() => {
        if (!isDropdownOpen) return;
        const onDocClick = (e: MouseEvent) => {
            if (dropdownRef.current && !dropdownRef.current.contains(e.target as Node)) {
                setIsDropdownOpen(false);
            }
        };
        document.addEventListener('mousedown', onDocClick);
        return () => document.removeEventListener('mousedown', onDocClick);
    }, [isDropdownOpen]);

    const selectedPlaybook = useMemo(
        () => playbooks?.find((p) => p.id === selectedId) ?? null,
        [playbooks, selectedId],
    );

    // Only `ready` playbooks render their blocks; searching the others would give the
    // counter matches that never paint.
    const matches = useMemo(() => {
        if (!isFindOpen || selectedPlaybook?.status !== 'ready') return [];
        return findMatches(selectedPlaybook.blocks, query);
    }, [isFindOpen, selectedPlaybook, query]);

    const hasNoMatches = matches.length === 0;

    const suggestions = useMemo(() => {
        if (!isFindOpen || !query || !hasNoMatches) return [];
        return buildSuggestions(playbooks, selectedId, query);
    }, [isFindOpen, query, hasNoMatches, playbooks, selectedId]);

    // Adjusted during render, not in an effect: an effect would paint one stale frame
    // (e.g. "8/3") whenever a query narrows.
    const [lastSelectedId, setLastSelectedId] = useState(selectedId);
    const [lastQuery, setLastQuery] = useState(query);
    if (selectedId !== lastSelectedId) {
        setLastSelectedId(selectedId);
        setLastQuery(query);
        setActiveMatchIndex(0);
        setActiveSuggestionIndex(0);
    } else if (query !== lastQuery) {
        setLastQuery(query);
        setActiveMatchIndex(indexAtOrAfter(matches, matchAnchorRef.current));
        setActiveSuggestionIndex(0);
    }

    const effectiveMatchIndex = hasNoMatches ? 0 : Math.min(activeMatchIndex, matches.length - 1);
    const effectiveSuggestionIndex =
        suggestions.length === 0 ? 0 : Math.min(activeSuggestionIndex, suggestions.length - 1);

    useEffect(() => {
        const active = matches[effectiveMatchIndex];
        if (active) matchAnchorRef.current = active;
    }, [matches, effectiveMatchIndex]);

    const handleClose = () => {
        window.electron?.ipcRenderer?.send('close-playbook-window');
    };

    const handleSelect = (id: string) => {
        setSelectedId(id);
        setIsDropdownOpen(false);
        localStorage.setItem(LAST_USED_KEY, id);
    };

    const closeFind = () => {
        setIsFindOpen(false);
        setQuery('');
        matchAnchorRef.current = null;
    };

    const stepMatch = (delta: number) => {
        if (hasNoMatches) return;
        setActiveMatchIndex((effectiveMatchIndex + delta + matches.length) % matches.length);
    };

    const handleSuggestionSelect = (suggestion: PlaybookSuggestion) => {
        handleSelect(suggestion.playbookId);
        // A name-only hit has nothing to highlight, so the bar would sit on "0/0".
        if (suggestion.matchCount === 0) closeFind();
    };

    // All find keys live here rather than on the input, so they behave the same whether
    // focus sits in the find bar or back in the playbook body.
    useEffect(() => {
        const onKeyDown = (e: KeyboardEvent) => {
            const isAccel = e.metaKey || e.ctrlKey;

            if (isAccel && e.key.toLowerCase() === 'f') {
                e.preventDefault();
                if (e.repeat) return;
                setIsDropdownOpen(false);
                setIsFindOpen(true);
                setFindFocusToken((token) => token + 1);
                return;
            }

            if (e.key === 'Escape') {
                if (isDropdownOpen) setIsDropdownOpen(false);
                else if (isFindOpen) closeFind();
                return;
            }

            if (!isFindOpen) return;

            if (isAccel && e.key.toLowerCase() === 'g') {
                e.preventDefault();
                stepMatch(e.shiftKey ? -1 : 1);
                return;
            }

            if (suggestions.length > 0 && (e.key === 'ArrowDown' || e.key === 'ArrowUp')) {
                e.preventDefault();
                const delta = e.key === 'ArrowDown' ? 1 : -1;
                setActiveSuggestionIndex(
                    (effectiveSuggestionIndex + delta + suggestions.length) % suggestions.length,
                );
                return;
            }

            if (e.key === 'Enter') {
                e.preventDefault();
                if (suggestions.length > 0) {
                    const suggestion = suggestions[effectiveSuggestionIndex];
                    if (suggestion) handleSuggestionSelect(suggestion);
                    return;
                }
                stepMatch(e.shiftKey ? -1 : 1);
            }
        };
        window.addEventListener('keydown', onKeyDown);
        return () => window.removeEventListener('keydown', onKeyDown);
    }, [isFindOpen, isDropdownOpen, suggestions, effectiveSuggestionIndex, matches, effectiveMatchIndex]);

    const hasContent = !isLoading && !error && !!playbooks && playbooks.length > 0;

    return (
        <div className="playbook-window-container">
            <div ref={dropdownRef} className="playbook-header-region">
                <PlaybookHeader
                    selectedPlaybook={selectedPlaybook}
                    isDropdownOpen={isDropdownOpen}
                    showFindHint={hasContent && !isFindOpen}
                    onToggleDropdown={() => setIsDropdownOpen((v) => !v)}
                    onClose={handleClose}
                />
                {isDropdownOpen && playbooks && (
                    <PlaybookSelector
                        playbooks={playbooks}
                        selectedId={selectedId}
                        onSelect={handleSelect}
                    />
                )}
            </div>

            {isLoading && (
                <div className="playbook-state-message">
                    <Spinner width={18} height={18} />
                    <span>Loading playbooks…</span>
                </div>
            )}

            {error && !isLoading && (
                <div className="playbook-state-message playbook-state-error">
                    <span>{error}</span>
                </div>
            )}

            {!isLoading && !error && playbooks && playbooks.length === 0 && (
                <div className="playbook-state-message">
                    <span>No playbooks yet.</span>
                </div>
            )}

            {hasContent && (
                <div className={`playbook-content-region ${isFindOpen ? 'is-find-open' : ''}`}>
                    <PlaybookBody
                        playbook={selectedPlaybook}
                        matches={matches}
                        activeMatchIndex={effectiveMatchIndex}
                    />
                    {isFindOpen && (
                        <div className="playbook-find-layer">
                            <PlaybookFindBar
                                query={query}
                                matchCount={matches.length}
                                activeMatchIndex={effectiveMatchIndex}
                                focusToken={findFocusToken}
                                activeSuggestionId={
                                    suggestions.length > 0
                                        ? `playbook-suggestion-${suggestions[effectiveSuggestionIndex]?.playbookId}`
                                        : undefined
                                }
                                onQueryChange={setQuery}
                                onNext={() => stepMatch(1)}
                                onPrevious={() => stepMatch(-1)}
                                onClose={closeFind}
                            />
                            {query.length > 0 && hasNoMatches && (
                                <FindSuggestions
                                    suggestions={suggestions}
                                    activeIndex={effectiveSuggestionIndex}
                                    onSelect={handleSuggestionSelect}
                                />
                            )}
                        </div>
                    )}
                </div>
            )}

            <div className="playbook-resize-grip" />
        </div>
    );
}
