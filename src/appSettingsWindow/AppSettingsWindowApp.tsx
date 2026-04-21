import React, { useCallback, useEffect, useState } from "react";
import { useSessionExpiry } from "@/hooks/useSessionExpiry";
import { LuPersonStanding, LuSettings, LuZap } from "react-icons/lu";
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

export type SidebarOptionType = 'cue' | 'auto-stop' | 'audio' | 'scripts' | 'accessibility' | 'general';

interface SidebarOption {
    key: SidebarOptionType;
    icon: React.ReactNode;
    label: string;
}

const SIDEBAR_OPTIONS: SidebarOption[] = [
	{ key: 'general', label: 'General', icon: <LuSettings /> },
	{ key: 'cue', label: 'Cue', icon: <LuZap /> },
    { key: 'accessibility', label: 'Accessibility', icon: <LuPersonStanding /> },
    // { key: 'audio', label: 'Audio', icon: <LuHeadphones /> },
    // { key: 'scripts', label: 'Scripts', icon: <LuBookText /> },
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
    const [active, setActive] = useState<SidebarOptionType>('general');
    const [searchValue, setSearchValue] = useState('');
    const [highlightId, setHighlightId] = useState<string | null>(null);

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
    const closeWindow = useCallback(() => {
        if (window?.electron?.ipcRenderer) {
            window.electron.ipcRenderer.send('close-app-settings-window')
        }
    }, []);
    useSessionExpiry(closeWindow);

    return (
        <CoachSettingsProvider>
            <div className="app-settings-container">
                <AppSettingsContent />
            </div>
        </CoachSettingsProvider>
    );
}

export default AppSettingsWindowApp;
