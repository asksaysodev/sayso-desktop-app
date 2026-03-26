import { useEffect, useRef, useState } from "react";
import SettingsContentLayout from "./SettingsContentLayout";
import { CueMode } from "../types";
import useCoachSettingsContext from "../context/CoachSettingsContext";
import useDebounce from "@/hooks/useDebounce";

export default function CueSettings() {
    const [mode, setMode] = useState<CueMode>('condensed');
    const [bufferTime, setBufferTime] = useState<number | undefined>(undefined);
    const { coachSettings, mutateBufferTime, mutateCueMode } = useCoachSettingsContext();
    const serverValue = useRef<number | undefined>(undefined);
    const debouncedBufferTime = useDebounce(bufferTime, 1500);

    useEffect(() => {
        if (coachSettings?.insight_buffer_time_ms !== undefined) {
            const bufferTimeInSeconds = coachSettings.insight_buffer_time_ms / 1000;
            serverValue.current = bufferTimeInSeconds;
            setBufferTime(bufferTimeInSeconds);
        }

        if (coachSettings?.cue_mode) setMode(coachSettings?.cue_mode)
    }, [coachSettings]);

    useEffect(() => {
        if (debouncedBufferTime === undefined || debouncedBufferTime === serverValue.current) return;
        mutateBufferTime(debouncedBufferTime);
    }, [debouncedBufferTime]);
    
    const handleChangeCueMode = (mode: CueMode) => {
        setMode(mode);
        mutateCueMode(mode);
    }
    
    return (
        <SettingsContentLayout title="Cue">
            <div id="cue-mode" className="cue-setting-item">
                <div className="cue-setting-left">
                    <span className="cue-setting-label">Mode</span>
                    <p className="cue-setting-description">Choose how insights are displayed during your call.</p>
                </div>
                <div className="cue-setting-right">
                    <div className="cue-mode-toggle">
                        <button
                            className={`cue-mode-option ${mode === 'full' ? 'active' : ''}`}
                            onClick={() => handleChangeCueMode('full')}
                        >
                            Full Insights
                        </button>
                        <button
                            className={`cue-mode-option ${mode === 'condensed' ? 'active' : ''}`}
                            onClick={() => handleChangeCueMode('condensed')}
                        >
                            Condensed
                        </button>
                    </div>
                </div>
            </div>

            <div id="cue-buffer-time" className="cue-setting-item">
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
                            value={bufferTime ?? 0}
                            onChange={e => setBufferTime(Number(e.target.value))}
                            className="cue-range"
                        />
                        <span className="cue-range-value">{bufferTime ?? 0}s</span>
                    </div>
                </div>
            </div>
        </SettingsContentLayout>
    );
}
