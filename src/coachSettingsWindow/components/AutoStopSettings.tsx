import { useEffect, useRef, useState } from "react";
import SettingsContentLayout from "./SettingsContentLayout";
import useCoachSettingsContext from "../context/CoachSettingsContext";
import useDebounce from "@/hooks/useDebounce";

export default function AutoStopSettings() {
    const [timeDelay, setTimeDelay] = useState<number | undefined>(undefined);
    const { coachSettings, mutateAutoStopTimeDelay } = useCoachSettingsContext();
    const debouncedTimeDelay = useDebounce(timeDelay, 1500);
    const serverValue = useRef<number | undefined>(undefined);
    
    useEffect(() => {
        if (coachSettings?.auto_stop_delay_minutes !== undefined) {
            const autoStopDelayMinutes = coachSettings.auto_stop_delay_minutes;
            serverValue.current = autoStopDelayMinutes;
            setTimeDelay(autoStopDelayMinutes);
        }
    }, [coachSettings])
    
    useEffect(() => {
        if (debouncedTimeDelay === undefined || debouncedTimeDelay === serverValue.current) return;
        mutateAutoStopTimeDelay(debouncedTimeDelay);
    }, [debouncedTimeDelay]);
    
    return (
        <SettingsContentLayout title="Auto Stop">
            <div id="auto-stop-time-delay" className="cue-setting-item">
                <div className="cue-setting-left">
                    <span className="cue-setting-label">Time Delay</span>
                    <p className="cue-setting-description">How long after the call starts before Sayso automatically stops the session.</p>
                </div>
                <div className="cue-setting-right">
                    <div className="cue-range-row">
                        <input
                            type="range"
                            min={2}
                            max={10}
                            step={1}
                            value={timeDelay ?? 0}
                            onChange={e => setTimeDelay(Number(e.target.value))}
                            className="cue-range"
                        />
                        <span className="cue-range-value">{timeDelay ?? 0} min</span>
                    </div>
                </div>
            </div>
        </SettingsContentLayout>
    );
}
