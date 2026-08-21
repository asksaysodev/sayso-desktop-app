import { useEffect, useMemo, useRef, useState } from 'react';
import { Spinner } from '@/components/ui/spinner';
import { usePlaybooksCache } from './hooks/usePlaybooksCache';
import { usePlaybookPrefetch } from './hooks/usePlaybookPrefetch';
import { useOpenLastUsedSetting } from './hooks/useOpenLastUsedSetting';
import {
    PlaybookMatch,
    findMatches,
    indexAtOrAfter,
} from './helpers/searchPlaybook';
import PlaybookHeader from './components/PlaybookHeader';
import PlaybookSelector from './components/PlaybookSelector';
import PlaybookBody from './components/PlaybookBody';

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
    const [findFocusToken, setFindFocusToken] = useState(0);
    // Document position of the last active match, so refining a query resumes nearby.
    // Only consulted while a query is live — a search typed from empty starts at match 0.
    const matchAnchorRef = useRef<PlaybookMatch | null>(null);

    // Same order as Settings → Playbooks → Scripts: the server sorts by the
    // account's stored playbook_order, so no client-side re-sort here.
    const playbooks = rawPlaybooks;

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

    // Adjusted during render, not in an effect: an effect would paint one stale frame
    // (e.g. "8/3") whenever a query narrows.
    const [lastSelectedId, setLastSelectedId] = useState(selectedId);
    const [lastQuery, setLastQuery] = useState(query);
    if (selectedId !== lastSelectedId) {
        setLastSelectedId(selectedId);
        setLastQuery(query);
        setActiveMatchIndex(0);
    } else if (query !== lastQuery) {
        setLastQuery(query);
        // A search started from an empty field jumps to the top of the playbook;
        // refining a live query keeps the reader where they were (the nearest match
        // going forward), so typing one more character doesn't yank them back up.
        setActiveMatchIndex(lastQuery ? indexAtOrAfter(matches, matchAnchorRef.current) : 0);
    }

    const effectiveMatchIndex = hasNoMatches ? 0 : Math.min(activeMatchIndex, matches.length - 1);

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

    const clearQuery = () => {
        setQuery('');
        matchAnchorRef.current = null;
    };

    // A query belongs to the playbook it was typed against, so it's dropped when the
    // selector opens rather than carried over to whichever playbook is picked next.
    const toggleSelectorDropdown = () => {
        setIsDropdownOpen((isOpen) => {
            const next = !isOpen;
            if (next) clearQuery();
            return next;
        });
    };

    const stepMatch = (delta: number) => {
        if (hasNoMatches) return;
        setActiveMatchIndex((effectiveMatchIndex + delta + matches.length) % matches.length);
    };

    const hasContent = !isLoading && !error && !!playbooks && playbooks.length > 0;

    // All find keys live here rather than on the input, so they behave the same whether
    // focus sits in the search field or back in the playbook body.
    useEffect(() => {
        const onKeyDown = (e: KeyboardEvent) => {
            const isAccel = e.metaKey || e.ctrlKey;

            if (isAccel && e.key.toLowerCase() === 'f') {
                e.preventDefault();
                // Nothing to focus yet while playbooks are still loading — bumping the token
                // here would fire the focus effect before the search input has mounted, and
                // it wouldn't retry once it does (the effect only reruns on focusToken).
                if (e.repeat || !hasContent) return;
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

            if (e.key === 'Enter') {
                e.preventDefault();
                stepMatch(e.shiftKey ? -1 : 1);
            }
        };
        window.addEventListener('keydown', onKeyDown);
        return () => window.removeEventListener('keydown', onKeyDown);
    }, [query, isDropdownOpen, matches, effectiveMatchIndex, hasContent]);

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
                    onToggleDropdown={toggleSelectorDropdown}
                    onQueryChange={setQuery}
                    onQueryFocus={() => setIsDropdownOpen(false)}
                    onNext={() => stepMatch(1)}
                    onPrevious={() => stepMatch(-1)}
                    onClearQuery={clearQuery}
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
                <div className="playbook-content-region">
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
