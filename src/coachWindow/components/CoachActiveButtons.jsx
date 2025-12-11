import { FaPause, FaStop } from "react-icons/fa6";
import { GrPowerReset } from "react-icons/gr";
import { LuLoader } from "react-icons/lu";
import CallTimer from "./CallTimer";
import { useCoachWindowStore } from "../../store/coachWindowStore";

export default function CoachActiveButtons({ coachFeature, handleCoach }) {
    const addInsight = useCoachWindowStore(state => state.cue_addInsight);
    const isCoachLoading = useCoachWindowStore(state => state.isCoachLoading);
    const isResettingCueSession = useCoachWindowStore(state => state.cue.isResettingCueSession);
    const cue_handleStopCue = useCoachWindowStore(state => state.cue_handleStopCue);
    const cue_handleStartCue = useCoachWindowStore(state => state.cue_handleStartCue);
    const resetCallDuration = useCoachWindowStore(state => state.resetCallDuration);
    const cue_onPressResetSession = useCoachWindowStore(state => state.cue_onPressResetSession);

    const showTimer = coachFeature === 'recall' || coachFeature === 'cue';
    const disableButtons = isCoachLoading || isResettingCueSession;
    return (
        <>
            {coachFeature === 'cue' && 
                <button 
                    className="coach-button reset"
                    onClick={cue_onPressResetSession} 
                    disabled={disableButtons}
                >
                    {isResettingCueSession && (
                        <LuLoader className="loading-spinner" />
                    )}
                    <GrPowerReset className={`${isResettingCueSession ? 'hidden' : ''}`} />
                </button>
            } 
            {/* <button className="coach-button pause" onClick={handleCoach}>
                <FaPause/>
            </button> */}
            <button
                className={`coach-button stop ${isCoachLoading ? 'loading' : ''}`}
                onClick={handleCoach} 
                disabled={disableButtons}
            >
                {isCoachLoading && (
                    <LuLoader className="loading-spinner" />
                )}
                <FaStop className={`${isCoachLoading ? 'hidden' : ''}`} />
            </button>

            {showTimer && 
                <CallTimer shouldStopTimer={disableButtons}/>
            }
        </>
    )
}