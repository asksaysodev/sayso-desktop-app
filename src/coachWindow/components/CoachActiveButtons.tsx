import { FaStop } from "react-icons/fa6";
import { GrPowerReset } from "react-icons/gr";
import { LuEye, LuEyeClosed, LuLoader } from "react-icons/lu";
import CallTimer from "./CallTimer";
import { useCoachWindowStore } from "../../store/coachWindowStore";
import { CoachFeature } from "@/types/store/coachWindowStore";

const ICON_SIZE = 15;

interface Props {
    coachFeature: CoachFeature;
    handleCoach: () => void;
    isInsightsLayoutOpen: boolean;
    setIsInsightsLayoutOpen: (isOpen: boolean) => void;
}

export default function CoachActiveButtons({ coachFeature, handleCoach, isInsightsLayoutOpen, setIsInsightsLayoutOpen }: Props) {
    const isCoachLoading = useCoachWindowStore(state => state.isCoachLoading);
    const isResettingCueSession = useCoachWindowStore(state => state.cue.isResettingCueSession);
    const cue_onPressResetSession = useCoachWindowStore(state => state.cue_onPressResetSession);
    const unseenInsightsCount = useCoachWindowStore(state => state.cue.unseenInsightsCount);
    const resetUnseenInsightsCount = useCoachWindowStore(state => state.cue_resetUnseenInsightsCount);

    const showTimer = coachFeature === 'recall' || coachFeature === 'cue';
    const disableButtons = isCoachLoading || isResettingCueSession;

    const handleOpenInsightsLayout = () => {
        if (!isInsightsLayoutOpen) {
            resetUnseenInsightsCount();
        }
        setIsInsightsLayoutOpen(!isInsightsLayoutOpen);
    }

    return (
        <>
            {coachFeature === 'cue' && 
                <button 
                className={`coach-button reset ${isResettingCueSession ? 'loading' : ''}`}
                onClick={cue_onPressResetSession} 
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
            {coachFeature === 'cue' && 
                <button 
                    className='open-insights-vertical-layout-button'
                    onClick={handleOpenInsightsLayout}
                    disabled={disableButtons}
                >
                    {unseenInsightsCount > 0 && <span className="cue-eye-toggle-notification-badge"></span>}
                    {isInsightsLayoutOpen ? <LuEyeClosed size={ICON_SIZE}/> : <LuEye size={ICON_SIZE}/>}
                </button>
            }
        </>
    )
}