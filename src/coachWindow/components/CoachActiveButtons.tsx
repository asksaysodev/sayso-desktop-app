import { FaStop } from "react-icons/fa6";
import { GrPowerReset } from "react-icons/gr";
import { LuLoader } from "react-icons/lu";
import CallTimer from "./CallTimer";
import { useCoachWindowStore } from "../../store/coachWindowStore";
import { CoachFeature } from "@/types/store/coachWindowStore";

const ICON_SIZE = 15;

interface Props {
    coachFeature: CoachFeature;
    handleCoach: () => void;
    onRequestReset?: () => void;
}

export default function CoachActiveButtons({ coachFeature, handleCoach, onRequestReset }: Props) {
    const isCoachLoading = useCoachWindowStore(state => state.isCoachLoading);
    const isResettingCueSession = useCoachWindowStore(state => state.cue.isResettingCueSession);

    const showTimer = coachFeature === 'recall' || coachFeature === 'cue';
    const disableButtons = isCoachLoading || isResettingCueSession;

    return (
        <>
            {coachFeature === 'cue' &&
                <button
                className={`coach-button reset ${isResettingCueSession ? 'loading' : ''}`}
                onClick={onRequestReset}
                disabled={disableButtons}
                >
                    {isResettingCueSession && (
                        <LuLoader size={ICON_SIZE} className="loading-spinner" />
                    )}
                    <GrPowerReset size={ICON_SIZE} className={`${isResettingCueSession ? 'hidden' : ''}`} />
                </button>
            } 
            <button
                className={`coach-button stop ${isCoachLoading ? 'loading' : ''}`}
                onClick={handleCoach} 
                disabled={disableButtons}
                >
                {isCoachLoading && (
                    <LuLoader size={ICON_SIZE} className="loading-spinner" />
                )}
                <FaStop size={ICON_SIZE} className={`${isCoachLoading ? 'hidden' : ''}`} />
            </button>

            {showTimer &&
                <CallTimer shouldStopTimer={disableButtons}/>
            }
        </>
    )
}