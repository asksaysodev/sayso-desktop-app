import React from "react";
import { FaPause, FaStop } from "react-icons/fa6";
import { GrPowerReset } from "react-icons/gr";
import { useCoachWindowContext } from "../../context/CoachWindowContext";

// Function to format seconds into h:mm:ss format
const formatDuration = (totalSeconds) => {
    const hours = Math.floor(totalSeconds / 3600);
    const minutes = Math.floor((totalSeconds % 3600) / 60);
    const seconds = totalSeconds % 60;
    
    return `${hours}:${minutes.toString().padStart(2, '0')}:${seconds.toString().padStart(2, '0')}`;
};

export default function CoachActiveButtons({ coachFeature, handleCoach, callDurationInSeconds }) {
    const { addInsight } = useCoachWindowContext();

    const showTimer = coachFeature === 'recall' || coachFeature === 'cue';

    function handleResetCue() {
        console.log('handleResetCue called');
        addInsight({
            message: 'Reset Cue',
            priority: 'high'
        });
    }

    return (
        <>
            {coachFeature === 'cue' && 
                <button className="coach-button reset" onClick={handleResetCue}>
                    <GrPowerReset/>
                </button>
            }
            {/* <button className="coach-button pause" onClick={handleCoach}>
                <FaPause/>
            </button> */}
            <button className="coach-button stop" onClick={handleCoach}>
                <FaStop/>
            </button>

            {showTimer && 
                <div className="call-duration-container">
                    <p className="call-duration">{formatDuration(callDurationInSeconds)}</p>
                </div>
            }
        </>
    )
}