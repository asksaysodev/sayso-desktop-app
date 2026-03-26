import React, { useEffect } from "react";
import { useCoachWindowStore } from "../../store/coachWindowStore";

// Function to format seconds into h:mm:ss format
const formatDuration = (totalSeconds: number): string => {
    const hours = Math.floor(totalSeconds / 3600);
    const minutes = Math.floor((totalSeconds % 3600) / 60);
    const seconds = totalSeconds % 60;
    
    return `${hours}:${minutes.toString().padStart(2, '0')}:${seconds.toString().padStart(2, '0')}`;
};

export default function CallTimer({ shouldStopTimer }: { shouldStopTimer: boolean }) {
    const callDurationInSeconds = useCoachWindowStore(state => state.callDurationInSeconds);
    const isCoachActive = useCoachWindowStore(state => state.isCoachActive);
    const incrementCallDuration = useCoachWindowStore(state => state.incrementCallDuration);
    const resetCallDuration = useCoachWindowStore(state => state.resetCallDuration);

    useEffect(() => {
        if (!isCoachActive) {
            resetCallDuration();
            return;
        }
        if (!shouldStopTimer) {
            const interval = setInterval(() => {
                incrementCallDuration();
            }, 1000);
            return () => clearInterval(interval);
        }
        // isCoachActive but shouldStopTimer=true (e.g. stop in progress) — pause timer, don't reset
    }, [isCoachActive, shouldStopTimer])

    return (
        <div className="call-duration-container">
            <p className="call-duration">
                {formatDuration(callDurationInSeconds)}
            </p>
        </div>
    )
}