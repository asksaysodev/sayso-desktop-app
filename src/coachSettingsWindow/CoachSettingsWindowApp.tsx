import React, { useEffect, useState } from "react";
import { LuBolt, LuBookText, LuHeadphones, LuPause } from "react-icons/lu";
import CueSettings from "./components/CueSettings";
import AutoStopSettings from "./components/AutoStopSettings";
import ScriptsSettings from "./components/ScriptsSettings";
import AudioSettings from "./components/AudioSettings";
import TopDragBar from "@/components/TopDragBar";
import { searchSettings, SettingsRegistryEntry } from "./settingsRegistry";
import CoachSettingsSearchBar from "./components/CoachSettingsSearchBar";
import { CoachSettingsProvider } from "./context/CoachSettingsContext";
import useCoachSettingsContext from "./context/CoachSettingsContext";

export type SidebarOptionType = 'cue' | 'auto-stop' | 'audio' | 'scripts';

interface SidebarOption {
    key: SidebarOptionType;
    icon: React.ReactNode;
    label: string;
}

const SIDEBAR_OPTIONS: SidebarOption[] = [
    { key: 'cue', label: 'Cue', icon: <LuBolt /> },
    { key: 'auto-stop', label: 'Auto Stop', icon: <LuPause /> },
    // { key: 'audio', label: 'Audio', icon: <LuHeadphones /> },
    // { key: 'scripts', label: 'Scripts', icon: <LuBookText /> },
];

function CoachSettingsContent() {
    const { coachSettingsIsLoading } = useCoachSettingsContext();
    const [active, setActive] = useState<SidebarOptionType>('cue');
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
            case "auto-stop": return <AutoStopSettings />
            case "audio": return <AudioSettings />
            case "scripts": return <ScriptsSettings />
        }
    };

    if (coachSettingsIsLoading) return null;

    return (
        <>
            <TopDragBar />

            <div className="coach-settings-sidebar">
                <CoachSettingsSearchBar onChangeText={setSearchValue} value={searchValue} />
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

            <div className="coach-settings-active-content-container">
                {renderContent()}
            </div>
        </>
    );
}

function CoachSettingsWindowApp() {
    return (
        <CoachSettingsProvider>
            <div className="coach-settings-container">
                <CoachSettingsContent />
            </div>
        </CoachSettingsProvider>
    );
}

export default CoachSettingsWindowApp;
