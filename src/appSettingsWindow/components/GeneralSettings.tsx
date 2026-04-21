import { useEffect, useRef, useState } from "react";
import SettingsContentLayout from "./SettingsContentLayout";
import useCoachSettingsContext from "../context/CoachSettingsContext";
import useDebounce from "@/hooks/useDebounce";
import SettingsToggle from "./SettingsToggle";

export default function GeneralSettings() {
    const [timeDelay, setTimeDelay] = useState<number | undefined>(undefined);
    const [launchAtLogin, setLaunchAtLogin] = useState<boolean | null>(null);
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

    useEffect(() => {
        window.electron?.ipcRenderer?.invoke('get-launch-at-login')
            .then((enabled: unknown) => {
                setLaunchAtLogin(enabled === true);
            });
    }, []);

    const handleLaunchAtLoginToggle = (value: 'enabled' | 'disabled') => {
        const enabled = value === 'enabled';
        setLaunchAtLogin(enabled);
        window.electron?.ipcRenderer?.invoke('set-launch-at-login', enabled);
    };

    if (launchAtLogin === null) return null;

    return (
        <SettingsContentLayout title="General">
            <div id="launch-at-login" className="cue-setting-item">
                <div className="cue-setting-left">
                    <span className="cue-setting-label">Launch at Login</span>
                    <p className="cue-setting-description">Automatically start Sayso in the tray when your Mac starts</p>
                </div>
                <div className="cue-setting-right">
                    <SettingsToggle
                        options={[
                            { value: 'disabled', label: 'Disabled' },
                            { value: 'enabled', label: 'Enabled' },
                        ]}
                        value={launchAtLogin ? 'enabled' : 'disabled'}
                        onChange={handleLaunchAtLoginToggle}
                    />
                </div>
            </div>
            <div id="auto-stop-time-delay" className="cue-setting-item">
                <div className="cue-setting-left">
                    <span className="cue-setting-label">Auto Stop</span>
                    <p className="cue-setting-description">How long after the call starts before Sayso automatically stops the session</p>
                </div>
                <div className="cue-setting-right">
                    <div className="cue-range-row">
                        <input
                            type="range"
                            min={2}
                            max={10}
                            step={1}
                            value={timeDelay ?? 2}
                            onChange={e => setTimeDelay(Number(e.target.value))}
                            className="cue-range"
                        />
                        <span className="cue-range-value">{timeDelay ?? 2} min</span>
                    </div>
                </div>
            </div>
        </SettingsContentLayout>
    );
}
