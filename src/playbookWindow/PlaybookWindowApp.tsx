import { useEffect, useLayoutEffect, useMemo, useRef, useState } from 'react';
import type { CSSProperties } from 'react';
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
import FindSuggestions from './components/FindSuggestions';

// Gap between the results dropdown and the highlighted text it would otherwise sit on top of.
const RESULTS_GAP_PX = 12;

const LAST_USED_KEY = 'sayso:lastUsedPlaybookId';

export default function PlaybookWindowApp() {
    usePlaybookPrefetch();
    const { playbooks: rawPlaybooks, error, isLoading } = usePlaybooksCache();
    const { openLastUsed, loaded: settingLoaded } = useOpenLastUsedSetting();
    const [selectedId, setSelectedId] = useState<string | null>(null);
    const [isDropdownOpen, setIsDropdownOpen] = useState(false);
    const dropdownRef = useRef<HTMLDivElement | null>(null);
    const [query, setQuery] = useState('');
    const [activeMatchIndex, setActiveMatchIndex] = useState(0);
    const [activeSuggestionIndex, setActiveSuggestionIndex] = useState(0);
    const [findFocusToken, setFindFocusToken] = useState(0);
    // Document position of the last active match, so narrowing a query resumes nearby.
    const matchAnchorRef = useRef<PlaybookMatch | null>(null);
    const resultsRef = useRef<HTMLDivElement | null>(null);
    const [resultsOffset, setResultsOffset] = useState(0);

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
        if (selectedPlaybook?.status !== 'ready') return [];
        return findMatches(selectedPlaybook.blocks, query);
    }, [selectedPlaybook, query]);

    const hasNoMatches = matches.length === 0;

    // Live, not just a zero-match fallback: a query can be relevant to other playbooks
    // even while the open one also has hits.
    const suggestions = useMemo(() => {
        if (!query) return [];
        return buildSuggestions(playbooks, selectedId, query);
    }, [query, playbooks, selectedId]);

    // Suppressed when the open playbook already has matches and no other playbook is
    // relevant — an empty "no other results" panel would be pure noise in that case.
    const showResults = query.length > 0 && (hasNoMatches || suggestions.length > 0);

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

    // Reserves room in the scrollable body so the results dropdown (which floats over the
    // top of it, anchored to the header) never covers an active highlighted match. Measured
    // rather than a fixed constant because the dropdown's height varies with result count;
    // animated via the `.playbook-body` transition so the growth/shrink isn't a hard jump.
    useLayoutEffect(() => {
        if (!showResults) {
            setResultsOffset(0);
            return;
        }
        const height = resultsRef.current?.offsetHeight ?? 0;
        setResultsOffset(height > 0 ? height + RESULTS_GAP_PX : 0);
    }, [showResults, suggestions, hasNoMatches]);

    const handleClose = () => {
        window.electron?.ipcRenderer?.send('close-playbook-window');
    };

    const handleSelect = (id: string) => {
        setSelectedId(id);
        setIsDropdownOpen(false);
        localStorage.setItem(LAST_USED_KEY, id);
    };

    const clearQuery = () => {
        setQuery('');
        matchAnchorRef.current = null;
    };

    const stepMatch = (delta: number) => {
        if (hasNoMatches) return;
        setActiveMatchIndex((effectiveMatchIndex + delta + matches.length) % matches.length);
    };

    const handleSuggestionSelect = (suggestion: PlaybookSuggestion) => {
        handleSelect(suggestion.playbookId);
        // Always closes the dropdown, not just for name-only hits: otherwise, after jumping
        // to a content match, the query stays live and the dropdown reopens over the new
        // playbook (now listing whatever still matches, possibly including the one you just
        // left) instead of just landing you on the result you picked.
        clearQuery();
    };

    // All find keys live here rather than on the input, so they behave the same whether
    // focus sits in the search field or back in the playbook body.
    useEffect(() => {
        const onKeyDown = (e: KeyboardEvent) => {
            const isAccel = e.metaKey || e.ctrlKey;

            if (isAccel && e.key.toLowerCase() === 'f') {
                e.preventDefault();
                if (e.repeat) return;
                setIsDropdownOpen(false);
                setFindFocusToken((token) => token + 1);
                return;
            }

            if (e.key === 'Escape') {
                if (isDropdownOpen) setIsDropdownOpen(false);
                else if (query) clearQuery();
                return;
            }

            if (!query) return;

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
    }, [query, isDropdownOpen, suggestions, effectiveSuggestionIndex, matches, effectiveMatchIndex]);

    const hasContent = !isLoading && !error && !!playbooks && playbooks.length > 0;

    return (
        <div className="playbook-window-container">
            <div ref={dropdownRef} className="playbook-header-region">
                <PlaybookHeader
                    selectedPlaybook={selectedPlaybook}
                    isDropdownOpen={isDropdownOpen}
                    hasContent={hasContent}
                    query={query}
                    matchCount={matches.length}
                    activeMatchIndex={effectiveMatchIndex}
                    hasMatches={!hasNoMatches}
                    focusToken={findFocusToken}
                    activeSuggestionId={
                        suggestions.length > 0
                            ? `playbook-suggestion-${suggestions[effectiveSuggestionIndex]?.playbookId}`
                            : undefined
                    }
                    onToggleDropdown={() => setIsDropdownOpen((v) => !v)}
                    onQueryChange={setQuery}
                    onQueryFocus={() => setIsDropdownOpen(false)}
                    onNext={() => stepMatch(1)}
                    onPrevious={() => stepMatch(-1)}
                    onClose={handleClose}
                />
                {isDropdownOpen && playbooks && (
                    <PlaybookSelector
                        playbooks={playbooks}
                        selectedId={selectedId}
                        onSelect={handleSelect}
                    />
                )}
                {showResults && (
                    <div ref={resultsRef} className="playbook-search-results">
                        <FindSuggestions
                            suggestions={suggestions}
                            activeIndex={effectiveSuggestionIndex}
                            onSelect={handleSuggestionSelect}
                        />
                    </div>
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
                <div
                    className="playbook-content-region"
                    style={{ '--pbw-search-offset': `${resultsOffset}px` } as CSSProperties}
                >
                    <PlaybookBody
                        playbook={selectedPlaybook}
                        matches={matches}
                        activeMatchIndex={effectiveMatchIndex}
                    />
                </div>
            )}

            <div className="playbook-resize-grip" />
        </div>
    );
}
