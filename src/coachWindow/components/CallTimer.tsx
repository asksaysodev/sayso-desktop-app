import React, { useEffect } from "react";
import { useCoachWindowStore } from "../../store/coachWindowStore";

// Function to format seconds into h:mm:ss format
const formatDuration = (totalSeconds) => {
    const hours = Math.floor(totalSeconds / 3600);
    const minutes = Math.floor((totalSeconds % 3600) / 60);
    const seconds = totalSeconds % 60;
    
    return `${hours}:${minutes.toString().padStart(2, '0')}:${seconds.toString().padStart(2, '0')}`;
};

export default function CallTimer() {
    const callDurationInSeconds = useCoachWindowStore(state => state.callDurationInSeconds);
    const isCoachActive = useCoachWindowStore(state => state.isCoachActive);
    const incrementCallDuration = useCoachWindowStore(state => state.incrementCallDuration);
    const resetCallDuration = useCoachWindowStore(state => state.resetCallDuration);

    useEffect(() => {
        if(isCoachActive) {
            const interval = setInterval(() => {
                incrementCallDuration();
            }, 1000)
            return () => clearInterval(interval)
        } else {
            resetCallDuration()
        }
    }, [isCoachActive])

    return (
        <div className="call-duration-container">
            <p className="call-duration">
                {formatDuration(callDurationInSeconds)}
            </p>
        </div>
    )
}