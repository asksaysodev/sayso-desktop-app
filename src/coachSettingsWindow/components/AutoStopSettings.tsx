import { useState } from "react";
import SettingsContentLayout from "./SettingsContentLayout";

export default function AutoStopSettings() {
    const [timeDelay, setTimeDelay] = useState<number>(3);

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
                            min={1}
                            max={5}
                            step={1}
                            value={timeDelay}
                            onChange={e => setTimeDelay(Number(e.target.value))}
                            className="cue-range"
                        />
                        <span className="cue-range-value">{timeDelay}m</span>
                    </div>
                </div>
            </div>
        </SettingsContentLayout>
    );
}
