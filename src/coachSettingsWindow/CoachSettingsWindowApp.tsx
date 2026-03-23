import React, { useState } from "react";
import { LuBolt, LuBookText, LuHeadphones, LuPause, LuSearch } from "react-icons/lu";
import CueSettings from "./components/CueSettings";
import AutoStopSettings from "./components/AutoStopSettings";
import ScriptsSettings from "./components/ScriptsSettings";
import TopDragBar from "@/components/TopDragBar";

type SidebarOptionType = 'cue' | 'auto-stop' | 'audio' | 'scripts';

interface SidebarOption {
    key: SidebarOptionType;
    icon: React.ReactNode;
    label: string;
}

const SIDEBAR_OPTIONS: SidebarOption[] = [
    {
        key: 'cue',
        label: 'Cue',
        icon: <LuBolt />
    },
    {
        key: 'auto-stop',
        label: 'Auto Stop',
        icon: <LuPause />
    },
    {
        key: 'audio',
        label: 'Audio',
        icon: <LuHeadphones />
    },
    {
        key: 'scripts',
        label: 'Scripts',
        icon: <LuBookText />
    },
];

function CoachSettingsWindowApp() {
    const [active, setActive] = useState<SidebarOptionType>('cue');
    
    const renderContent = () => {
        switch(active) {
            case "cue": return <CueSettings />
            case "auto-stop": return <AutoStopSettings />
            case "audio": return null
            case "scripts": return <ScriptsSettings />
        }
    }
    
    return (
        <div className="coach-settings-container">
            <TopDragBar />
            <div className="coach-settings-sidebar">
                <div className="sidebar-search-wrapper">
                    <LuSearch className="sidebar-search-icon" />
                    <input className="sidebar-search" placeholder="Search" />
                </div>
                {SIDEBAR_OPTIONS.map(({icon, label, key}) => (
                    <div 
                        key={key} 
                        className={`sidebar-option-button ${active === key ? 'active' : ''}`} 
                        onClick={() => setActive(key)}
                    >
                        {icon}
                        <p>{label}</p>
                    </div>
                ))}
            </div>
            <div className="coach-settings-active-content-container">
                {renderContent()}
            </div>
        </div>
    );
}

export default CoachSettingsWindowApp;
