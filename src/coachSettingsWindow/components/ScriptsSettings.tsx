import { useState, useRef, useEffect } from "react";
import { LuChevronDown } from "react-icons/lu";
import SettingsContentLayout from "./SettingsContentLayout";

const SCRIPT_OPTIONS = [
    { id: 1, label: 'Option 1' },
    { id: 2, label: 'Option 2' },
    { id: 3, label: 'Option 3' },
];

export default function ScriptsSettings() {
    const [active, setActive] = useState(false);
    const [selectedScript, setSelectedScript] = useState(SCRIPT_OPTIONS[0]);
    const [dropdownOpen, setDropdownOpen] = useState(false);
    const dropdownRef = useRef<HTMLDivElement>(null);

    useEffect(() => {
        function handleClickOutside(e: MouseEvent) {
            if (dropdownRef.current && !dropdownRef.current.contains(e.target as Node)) {
                setDropdownOpen(false);
            }
        }
        document.addEventListener('mousedown', handleClickOutside);
        return () => document.removeEventListener('mousedown', handleClickOutside);
    }, []);

    return (
        <SettingsContentLayout title="Scripts">
            <div id="scripts-active" className="cue-setting-item">
                <div className="cue-setting-left">
                    <span className="cue-setting-label">Active</span>
                    <p className="cue-setting-description">Enable or disable script guidance during your call.</p>
                </div>
                <div className="cue-setting-right">
                    <button
                        className={`settings-toggle ${active ? 'on' : ''}`}
                        onClick={() => setActive(v => !v)}
                        role="switch"
                        aria-checked={active}
                    >
                        <span className="settings-toggle-thumb" />
                    </button>
                </div>
            </div>
            <div id="scripts-script" className="cue-setting-item">
                <div className="cue-setting-left">
                    <span className="cue-setting-label">Script</span>
                    <p className="cue-setting-description">Choose which script to use during your call.</p>
                </div>
                <div className="cue-setting-right">
                    <div className="settings-dropdown" ref={dropdownRef}>
                        <button
                            className="settings-dropdown-trigger"
                            onClick={() => setDropdownOpen(v => !v)}
                        >
                            <span>{selectedScript.label}</span>
                            <LuChevronDown className={`settings-dropdown-chevron ${dropdownOpen ? 'open' : ''}`} />
                        </button>
                        {dropdownOpen && (
                            <div className="settings-dropdown-menu">
                                {SCRIPT_OPTIONS.map(option => (
                                    <button
                                        key={option.id}
                                        className={`settings-dropdown-item ${selectedScript.id === option.id ? 'active' : ''}`}
                                        onClick={() => {
                                            setSelectedScript(option);
                                            setDropdownOpen(false);
                                        }}
                                    >
                                        {option.label}
                                    </button>
                                ))}
                            </div>
                        )}
                    </div>
                </div>
            </div>
        </SettingsContentLayout>
    );
}
