import { LuLoader } from "react-icons/lu";
import CoachActiveButtons from "./CoachActiveButtons";
import { useCoachWindowStore } from "../../store/coachWindowStore";

export default function CoachButtons() {
    const isCoachLoading = useCoachWindowStore(state => state.isCoachLoading);
    const isCoachActive = useCoachWindowStore(state => state.isCoachActive);
    const coachFeature = useCoachWindowStore(state => state.coachFeature);

    const recall_startDualChannelRecording = useCoachWindowStore(state => state.recall_startDualChannelRecording);
    const recall_handleStopRecording = useCoachWindowStore(state => state.recall_handleStopRecording);
    const recall_selectedProspect = useCoachWindowStore(state => state.recall.selectedProspect);
    
    const cue_handleStartCue = useCoachWindowStore(state => state.cue_handleStartCue);
    const cue_handleStopCue = useCoachWindowStore(state => state.cue_handleStopCue);

    const COACH_ACTIONS = {
        recall: {
            start: async (prospect) => {
                if (!prospect) return;
                await recall_startDualChannelRecording(prospect.id);
            },
            stop: async () => {
                await recall_handleStopRecording();
            },
            validate: (prospect) => !!prospect && !!prospect.id,
        },
        cue: {
            start: async () => {
                await cue_handleStartCue();
            },
            stop: async () => {
				console.log('Stopping cue');
                await cue_handleStopCue();
            },
            validate: () => true,
        }
    }
    
    const handleCoach = async () => {
        try {
            if (!coachFeature || isCoachLoading) return;

            const actions = COACH_ACTIONS[coachFeature];
            if (!actions) return;

            if (isCoachActive) {
                await actions.stop();
            } else {
                if (!actions.validate(recall_selectedProspect)) return;
                await actions.start(recall_selectedProspect);
            }

        } catch (error) {
            console.error('Error in handleCoach:', error);
            // We could maybe show a toast
        }
    }

    return (
        <div className="coach-buttons-container">
            {
                isCoachActive ? (
                    <CoachActiveButtons coachFeature={coachFeature} handleCoach={handleCoach}/>
                ) : (
                    <>
                        <button className={`start-coach-button ${isCoachLoading ? 'loading' : ''}`} onClick={handleCoach}>
                            
                            {
                                isCoachLoading && (
                                    <LuLoader className="loading-spinner" />
                                )
                            }
                            <svg
                                xmlns="http://www.w3.org/2000/svg"
                                viewBox="0 0 58.78 80.89"
                                style={{ fill: "currentColor" }}
                                className={`${isCoachLoading ? 'hidden' : ''}`}
                            >
                                <g id="Layer_1-2" data-name="Layer 1">
                                    <g>
                                    <path d="M38.11,47.13c-.09.28-.27.54-.48.71.08-.08.12-.18.17-.28.08-.17.17-.31.31-.43Z" />
                                    <path d="M58.78,42.42c0,3.24-1.73,6.06-4.34,7.62l-.24.15-.09.05L6.09,80.67l-.07.04s-.02,0-.03,0c-.17.11-.37.17-.59.17-.65,0-1.17-.52-1.17-1.17,0-.28.1-.54.27-.75,0-.03,0-.06.02-.07l33-30.94s.07-.07.09-.1t0-.02c.22-.17.39-.43.48-.71h0c.05-.16.08-.32.08-.49,0-.9-.72-1.62-1.61-1.62H8.89c-4.9,0-8.89-3.99-8.89-8.89,0-3.38,1.88-6.32,4.66-7.83l.04-.02L49.23.2l.04-.03s.02,0,.03,0h0c.17-.1.37-.16.59-.16.64,0,1.17.52,1.17,1.17,0,.28-.1.55-.27.75,0,.03,0,.05-.02.06l-29.51,28.61s-.04.05-.05.07c-.25.19-.44.45-.53.77-.06.15-.09.32-.09.49,0,.89.72,1.61,1.61,1.61h27.69c4.91,0,8.89,3.99,8.89,8.9Z" />
                                    <path d="M38.11,47.13c-.09.28-.27.54-.48.71.08-.08.12-.18.17-.28.08-.17.17-.31.31-.43Z" />
                                    </g>
                                </g>
                            </svg>
                            <p className={`${isCoachLoading ? 'hidden' : ''}`}>Launch</p>
                        </button>
                    </>
                )
            }
        </div>
    )
}