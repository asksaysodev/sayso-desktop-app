import SettingsContentLayout from "./SettingsContentLayout";
import useCoachSettingsContext from "../context/CoachSettingsContext";
import SettingsToggle from "./SettingsToggle";

export default function PulseSettings() {
    const { coachSettings, mutatePulseEnabled } = useCoachSettingsContext();

    const pulseEnabled = coachSettings?.pulse_enabled ?? true;

    const handleToggle = (value: 'enabled' | 'disabled') => {
        mutatePulseEnabled(value === 'enabled');
    };

    return (
        <SettingsContentLayout title="Pulse">
            <div id="pulse-enabled" className="cue-setting-item">
                <div className="cue-setting-left">
                    <span className="cue-setting-label">Visible in Coach</span>
                    <p className="cue-setting-description">Show real-time market data in the Coach widget during your calls</p>
                </div>
                <div className="cue-setting-right">
                    <SettingsToggle
                        options={[
                            { value: 'disabled', label: 'Hidden' },
                            { value: 'enabled', label: 'Visible' },
                        ]}
                        value={pulseEnabled ? 'enabled' : 'disabled'}
                        onChange={handleToggle}
                    />
                </div>
            </div>
        </SettingsContentLayout>
    );
}
