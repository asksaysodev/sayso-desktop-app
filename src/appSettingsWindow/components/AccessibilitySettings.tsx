import SettingsContentLayout from "./SettingsContentLayout";
import { AccessibilityFontSizeType } from "../types";
import useCoachSettingsContext from "../context/CoachSettingsContext";
import { useEffect, useMemo, useState } from "react";

const FS_OPTIONS: { key: AccessibilityFontSizeType; label: string; previewSize: number }[] = [
    { key: 's', label: 'Small',  previewSize: 13.5 },
    { key: 'm', label: 'Medium', previewSize: 16.2 },
    { key: 'l', label: 'Large',  previewSize: 18.9 },
];

// Paints this window only. Main is told separately, and only once the server
// has accepted the change — see handleFontSizeChange.
function paintFontSize(size: AccessibilityFontSizeType) {
    if (size === 's') delete document.documentElement.dataset.fontSize;
    else document.documentElement.dataset.fontSize = size;
}

export default function AccessibilitySettings() {
    const [optimisticFontSize, setOptimisticFontSize] = useState<AccessibilityFontSizeType | null>(null);
    const [saveError, setSaveError] = useState(false);
    const { coachSettings, mutateUpdateFontSize } = useCoachSettingsContext();
    const selected = useMemo(() => optimisticFontSize ?? coachSettings?.font_size ?? 's', [optimisticFontSize,coachSettings])

    useEffect(() => {
        if (optimisticFontSize && coachSettings?.font_size === optimisticFontSize) {
            setOptimisticFontSize(null);
        }
    }, [coachSettings?.font_size, optimisticFontSize]);

    const handleFontSizeChange = (size: AccessibilityFontSizeType) => {
        const prev = selected;
        setOptimisticFontSize(size);
        setSaveError(false);
        paintFontSize(size);
        mutateUpdateFontSize(size, {
            // Main is told only after the server has accepted the value, so
            // what it caches, persists to disk and broadcasts is never
            // something the API rejected. This is also what stands its retry
            // ladder down for font size — a value the user just chose should
            // not be overwritten by a fetch that was already in flight.
            onSuccess: () => {
                window.electron?.ipcRenderer?.send('set-font-size', size);
            },
            onError: () => {
                setOptimisticFontSize(null);
                setSaveError(true);
                paintFontSize(prev);
            },
        });
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
                {saveError && <p style={{ color: 'red', fontSize: 12, marginTop: 4 }}>Failed to save. Please try again.</p>}
            </div>
        </SettingsContentLayout>
    );
}
