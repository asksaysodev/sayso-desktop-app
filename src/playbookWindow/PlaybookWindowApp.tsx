import { useEffect, useMemo, useRef, useState } from 'react';
import { Spinner } from '@/components/ui/spinner';
import { usePlaybooksCache } from './hooks/usePlaybooksCache';
import { usePlaybookPrefetch } from './hooks/usePlaybookPrefetch';
import PlaybookHeader from './components/PlaybookHeader';
import PlaybookSelector from './components/PlaybookSelector';
import PlaybookBody from './components/PlaybookBody';

export default function PlaybookWindowApp() {
    usePlaybookPrefetch();
    const { playbooks: rawPlaybooks, error, isLoading } = usePlaybooksCache();
    const [selectedId, setSelectedId] = useState<string | null>(null);
    const [isDropdownOpen, setIsDropdownOpen] = useState(false);
    const dropdownRef = useRef<HTMLDivElement | null>(null);

    const playbooks = useMemo(() => {
        if (!rawPlaybooks) return rawPlaybooks;
        return [...rawPlaybooks].sort((a, b) => Number(b.is_default) - Number(a.is_default));
    }, [rawPlaybooks]);

    useEffect(() => {
        if (!playbooks || playbooks.length === 0) {
            if (selectedId !== null) setSelectedId(null);
            return;
        }
        const stillExists = selectedId && playbooks.some((p) => p.id === selectedId);
        if (stillExists) return;
        const defaultReady = playbooks.find((p) => p.is_default && p.status === 'ready');
        const firstReady = playbooks.find((p) => p.status === 'ready');
        setSelectedId((defaultReady ?? firstReady ?? playbooks[0]).id);
    }, [playbooks, selectedId]);

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

    const handleClose = () => {
        window.electron?.ipcRenderer?.send('close-playbook-window');
    };

    const handleSelect = (id: string) => {
        setSelectedId(id);
        setIsDropdownOpen(false);
    };

    return (
        <div className="playbook-window-container">
            <div ref={dropdownRef} className="playbook-header-region">
                <PlaybookHeader
                    selectedPlaybook={selectedPlaybook}
                    isDropdownOpen={isDropdownOpen}
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

            {!isLoading && !error && playbooks && playbooks.length > 0 && (
                <PlaybookBody playbook={selectedPlaybook} />
            )}

            <div className="playbook-resize-grip" />
        </div>
    );
}
