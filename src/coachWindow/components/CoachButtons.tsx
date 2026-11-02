import CoachActiveButtons from "./CoachActiveButtons";
import { useCoachWindowStore } from "../../store/coachWindowStore";
import { useNetworkState } from "@/hooks/useNetworkState";
import { reportCoachError } from "@/utils/errorReporting";
import { LeadTypeEnum } from "@/types/coach";

const LEAD_START_BUTTONS = [
    { leadType: LeadTypeEnum.SELLER, label: 'Start for Seller' },
    { leadType: LeadTypeEnum.BUYER, label: 'Start for Buyer' },
];

interface Props {
    onRequestStop?: () => void;
    onRequestReset?: () => void;
}

export default function CoachButtons({ onRequestStop, onRequestReset }: Props) {
    const isCoachLoading = useCoachWindowStore(state => state.isCoachLoading);
    const isCoachActive = useCoachWindowStore(state => state.isCoachActive);
    const isCoachStarting = useCoachWindowStore(state => state.isCoachStarting);
    const coachFeature = useCoachWindowStore(state => state.coachFeature);
    const leadType = useCoachWindowStore(state => state.cue.leadType);
    const setLeadType = useCoachWindowStore(state => state.setLeadType);

    const cue_handleStartCue = useCoachWindowStore(state => state.cue_handleStartCue);
    const cue_handleStopCue = useCoachWindowStore(state => state.cue_handleStopCue);

    const { isReconnecting } = useNetworkState();
    // The lead being started: swaps the buttons for "Starting conversation"
    const startingLeadType = isCoachStarting && !isCoachActive ? leadType : null;
    // Stop turns isCoachActive off up front, so a stop in flight lands in the
    // idle branch too: "Saving conversation" until it's done
    const isStopping = isCoachLoading && !isCoachStarting && !isCoachActive;
    const isBusy = !!startingLeadType || isStopping;

    const COACH_ACTIONS = {
        cue: {
            start: async () => {
                await cue_handleStartCue();
            },
            stop: async () => {
                await cue_handleStopCue();
            },
            validate: () => true,
        }
    }
    
    // newLeadType: the Start button pressed while idle; absent for stop.
    const handleCoach = async (newLeadType?: LeadTypeEnum) => {
        try {
            // Call-time read: the render-closure isCoachLoading can be stale under a fast
            // double-click, letting two starts through (SAYSO-355). The subscribed value above
            // still drives the spinner/disabled UI.
            if (!coachFeature || useCoachWindowStore.getState().isCoachLoading) return;

            const actions = COACH_ACTIONS[coachFeature];
            if (!actions) return;

            if (isCoachActive) {
                if (onRequestStop) {
                    onRequestStop();
                    return;
                }
                await actions.stop();
            } else {
                // Backstop for the `disabled` prop below — the buttons are the only
                // callers, so offline can't get past here.
                if (isReconnecting || !newLeadType) return;
                if (!actions.validate()) return;
                setLeadType(newLeadType);
                await actions.start();
            }

        } catch (error) {
            console.error('Error in handleCoach:', error);
            reportCoachError(error);
        }
    }

    return (
        <div className="coach-buttons-container">
            {
                isCoachActive ? (
                    <CoachActiveButtons
                        coachFeature={coachFeature}
                        handleCoach={() => handleCoach()}
                        onRequestReset={onRequestReset}
                    />
                ) : (
                    <>
                        {/* Disabled while offline: start needs the network, and a bare
                            no-op click gave no feedback. An error can't be surfaced here —
                            CoachWindowMain renders coachWindowError only when !isReconnecting,
                            and shows its own "No internet connection" banner instead.
                            One button per lead type (SAYSO-566): picking one is the start.
                            While a session starts or stops, the buttons give way to a status
                            line (copy, not a button) and fade back in when it's done. */}
                        <div className={`lead-start${isBusy ? ' lead-start--busy' : ''}`}>
                            {LEAD_START_BUTTONS.map(({ leadType: type, label }) => (
                                <button
                                    key={type}
                                    className="lead-start-button"
                                    onClick={() => handleCoach(type)}
                                    disabled={isReconnecting || isCoachLoading}
                                >
                                    {label}
                                </button>
                            ))}
                            <span
                                className={`lead-start-status${startingLeadType ? ' lead-start-status--visible' : ''}`}
                                aria-hidden={!startingLeadType}
                            >
                                Starting conversation
                            </span>
                            <span
                                className={`lead-start-status${isStopping ? ' lead-start-status--visible' : ''}`}
                                aria-hidden={!isStopping}
                            >
                                Saving conversation
                            </span>
                        </div>
                    </>
                )
            }
        </div>
    )
}