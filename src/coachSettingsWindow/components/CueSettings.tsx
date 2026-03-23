import { useState } from "react";
import SettingsContentLayout from "./SettingsContentLayout";

type CueMode = 'full' | 'condensed';

export default function CueSettings() {
    const [mode, setMode] = useState<CueMode>('full');
    const [bufferTime, setBufferTime] = useState<number>(0);

    return (
        <SettingsContentLayout title="Cue">
            <div className="cue-setting-item">
                <div className="cue-setting-left">
                    <span className="cue-setting-label">Mode</span>
                    <p className="cue-setting-description">Choose how insights are displayed during your call.</p>
                </div>
                <div className="cue-setting-right">
                    <div className="cue-mode-toggle">
                        <button
                            className={`cue-mode-option ${mode === 'full' ? 'active' : ''}`}
                            onClick={() => setMode('full')}
                        >
                            Full Insights
                        </button>
                        <button
                            className={`cue-mode-option ${mode === 'condensed' ? 'active' : ''}`}
                            onClick={() => setMode('condensed')}
                        >
                            Condensed
                        </button>
                    </div>
                </div>
            </div>

            <div className="cue-setting-item">
                <div className="cue-setting-left">
                    <span className="cue-setting-label">Buffer Time</span>
                    <p className="cue-setting-description">How long to wait after the call starts before Sayso begins showing insights.</p>
                </div>
                <div className="cue-setting-right">
                    <div className="cue-range-row">
                        <input
                            type="range"
                            min={0}
                            max={30}
                            step={1}
                            value={bufferTime}
                            onChange={e => setBufferTime(Number(e.target.value))}
                            className="cue-range"
                        />
                        <span className="cue-range-value">{bufferTime}s</span>
                    </div>
                </div>
            </div>
        </SettingsContentLayout>
    );
}
