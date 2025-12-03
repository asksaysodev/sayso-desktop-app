import React from "react";
import { FaPause, FaStop } from "react-icons/fa6";
import { GrPowerReset } from "react-icons/gr";
import CallTimer from "./CallTimer";
import { useCoachWindowStore } from "../../store/coachWindowStore";

export default function CoachActiveButtons({ coachFeature, handleCoach }) {
    const addInsight = useCoachWindowStore(state => state.cue_addInsight);

    const showTimer = coachFeature === 'recall' || coachFeature === 'cue';

    function handleResetCue() {
        addInsight({
            message: 'I suggest that you tell him about the different payments methods. I suggest that you tell him about the different payments methods. I suggest that you tell him about the different payments methods. I suggest that you tell him about the different payments methods.',
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
                <CallTimer />
            }
        </>
    )
}