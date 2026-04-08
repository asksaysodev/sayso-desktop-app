import { useState } from "react";
import SettingsContentLayout from "./SettingsContentLayout";
import { AccessibilityFontSizeType } from "../types";
import useCoachSettingsContext from "../context/CoachSettingsContext";

const FS_OPTIONS: { key: AccessibilityFontSizeType; label: string; previewSize: number }[] = [
    { key: 's', label: 'Small',  previewSize: 13.5 },
    { key: 'm', label: 'Medium', previewSize: 16.2 },
    { key: 'l', label: 'Large',  previewSize: 18.9 },
];

export default function AccessibilitySettings() {
    const { coachSettings, mutateUpdateFontSize } = useCoachSettingsContext();
    const [selected, setSelected] = useState<AccessibilityFontSizeType>(coachSettings?.font_size ?? 's');

    const handleFontSizeChange = (size: AccessibilityFontSizeType) => {
        setSelected(size);
        mutateUpdateFontSize(size);
    }
    
    return (
        <SettingsContentLayout title="Accessibility">
            <div id="accessibility-settings" className="accessibility-setting-item">
                <div>
                    <span className="accessibility-setting-label">Font Size</span>
                    <p className="accessibility-setting-description">Choose the font size to use</p>
                </div>

                <div className="fs-flat-cards-container">
                    {FS_OPTIONS.map(({ key, label, previewSize }) => (
                        <button
                            key={key}
                            className={`fs-flat-card${selected === key ? ' active' : ''}`}
                            onClick={() => handleFontSizeChange(key)}
                        >
                            <span style={{ fontSize: previewSize, fontWeight: 600, lineHeight: 1, color: 'inherit' }}>aA</span>
                            <span className="fs-flat-card-label">{label}</span>
                        </button>
                    ))}
                </div>
            </div>
        </SettingsContentLayout>
    );
}
