import React, { useCallback, useEffect, useState } from "react";
import { useSessionExpiry } from "@/hooks/useSessionExpiry";
import { LuPersonStanding, LuSettings, LuZap } from "react-icons/lu";
import { Download } from "lucide-react";
import CueSettings from "./components/CueSettings";
import ScriptsSettings from "./components/ScriptsSettings";
import AudioSettings from "./components/AudioSettings";
import TopDragBar from "@/components/TopDragBar";
import { searchSettings, SettingsRegistryEntry } from "./settingsRegistry";
import AppSettingsSearchBar from "./components/AppSettingsSearchBar";
import { CoachSettingsProvider } from "./context/CoachSettingsContext";
import useCoachSettingsContext from "./context/CoachSettingsContext";
import AccessibilitySettings from "./components/AccessibilitySettings";
import GeneralSettings from "./components/GeneralSettings";
import { Book } from "lucide-react";
import PlaybooksSettings from "./components/PlaybooksSettings";
import SoftwareUpdateSettings from "./components/SoftwareUpdateSettings";

export type SidebarOptionType = 'cue' | 'auto-stop' | 'audio' | 'scripts' | 'accessibility' | 'general' | 'playbooks' | 'software-update';

interface SidebarOption {
    key: SidebarOptionType;
    icon: React.ReactNode;
    label: string;
}

const SIDEBAR_OPTIONS: SidebarOption[] = [
	{ key: 'general', label: 'General', icon: <LuSettings /> },
	{ key: 'cue', label: 'Cue', icon: <LuZap /> },
    { key: 'accessibility', label: 'Accessibility', icon: <LuPersonStanding /> },
    { key: 'playbooks', label: 'Playbooks', icon: <Book /> },
    { key: 'software-update', label: 'Software Update', icon: <Download size={16} /> },
];

function AppSettingsContent() {
    const { coachSettingsIsLoading, coachSettings } = useCoachSettingsContext();

    useEffect(() => {
        if (!coachSettings) return;
        const size = coachSettings.font_size;
        if (size === 's') {
            delete document.documentElement.dataset.fontSize;
        } else {
            document.documentElement.dataset.fontSize = size;
        }
        window.electron?.ipcRenderer?.send('set-font-size', size);
    }, [coachSettings?.font_size]);

    // Read initial tab from URL param (e.g. ?tab=software-update when opened from tray)
    const initialTab = (): SidebarOptionType => {
        const params = new URLSearchParams(window.location.search);
        const tab = params.get('tab') as SidebarOptionType | null;
        if (tab && SIDEBAR_OPTIONS.some(o => o.key === tab)) return tab;
        return 'general';
    };

    const [active, setActive] = useState<SidebarOptionType>(initialTab);
    const [searchValue, setSearchValue] = useState('');
    const [highlightId, setHighlightId] = useState<string | null>(null);

    // Navigate to software-update tab when main process requests it (e.g. tray click while window is open)
    useEffect(() => {
        const ipc = window.electron?.ipcRenderer;
        if (!ipc) return;
        const handler = () => setActive('software-update');
        ipc.on('app-settings:navigate-to-update', handler);
        return () => ipc.off('app-settings:navigate-to-update', handler);
    }, []);

    const searchResults: SettingsRegistryEntry[] = searchValue ? searchSettings(searchValue) : [];

    useEffect(() => {
        if (!highlightId) return;
        const el = document.getElementById(highlightId);
        if (!el) return;
        el.scrollIntoView({ behavior: 'smooth', block: 'center' });
        el.classList.add('highlight');
        const timer = setTimeout(() => el.classList.remove('highlight'), 1500);
        setHighlightId(null);
        return () => clearTimeout(timer);
    }, [active, highlightId]);

    const handleResultClick = (entry: SettingsRegistryEntry) => {
        setActive(entry.section);
        setHighlightId(entry.id);
    };

    const renderContent = () => {
        switch (active) {
            case "cue": return <CueSettings />
            case "audio": return <AudioSettings />
            case "scripts": return <ScriptsSettings />
            case "accessibility": return <AccessibilitySettings />
            case "general": return <GeneralSettings />
            case "playbooks": return <PlaybooksSettings />
            case "software-update": return <SoftwareUpdateSettings />
        }
    };

    if (coachSettingsIsLoading) return null;

    return (
        <>
            <TopDragBar />

            <div className="app-settings-sidebar">
                <AppSettingsSearchBar onChangeText={setSearchValue} value={searchValue} />
                {searchValue
                    ? searchResults.map((entry) => {
                        const sectionMeta = SIDEBAR_OPTIONS.find(o => o.key === entry.section)!;
                        return (
                            <div
                                key={entry.id}
                                className="sidebar-option-button"
                                onClick={() => handleResultClick(entry)}
                            >
                                {sectionMeta.icon}
                                <p>{entry.label}</p>
                            </div>
                        );
                    })
                    : SIDEBAR_OPTIONS.map(({ icon, label, key }) => (
                        <div
                            key={key}
                            className={`sidebar-option-button ${active === key ? 'active' : ''}`}
                            onClick={() => setActive(key)}
                        >
                            {icon}
                            <p>{label}</p>
                        </div>
                    ))
                }
            </div>

            <div className="app-settings-active-content-container">
                {renderContent()}
            </div>
        </>
    );
}

function AppSettingsWindowApp() {
    const handleSessionExpired = useCallback(() => {
        window.electron?.ipcRenderer?.send('app-settings:session-expired-redirect');
    }, []);
    useSessionExpiry(handleSessionExpired);

    return (
        <CoachSettingsProvider>
            <div className="app-settings-container">
                <AppSettingsContent />
            </div>
        </CoachSettingsProvider>
    );
}

export default AppSettingsWindowApp;
