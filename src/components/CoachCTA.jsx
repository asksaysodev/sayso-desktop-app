import {LuRocket} from 'react-icons/lu'

import '../styles/CoachCTA.css';
import { useCoachWindowStore } from '../store/coachWindowStore';

export default function CoachCTA({sidebar, active }) {
    const isCoachWindowOpen = useCoachWindowStore(state => state.isCoachWindowOpen);
    const openCoachWindow = useCoachWindowStore(state => state.openCoachWindow);
    const closeCoachWindow = useCoachWindowStore(state => state.closeCoachWindow);

    async function handleOnPressStartCoach() {
        if (isCoachWindowOpen) {
            closeCoachWindow();
        } else {
            try {
                await openCoachWindow()
            } catch (error) {
                console.error('Error in handleOnPressStartCoach:', error);
                // We could maybe show a toast
            }
        }
    }

    return (
        <div 
            className={`coach-cta-container ${sidebar ? 'sidebar' : ''} ${active ? 'active' : ''}`}
            onClick={handleOnPressStartCoach}
        >
            {
                !sidebar && (
                    <div className="outline"></div>
                )
            }
            <div className="main-button">
                <LuRocket />
                <p>{'Start AI Coach' }</p>
            </div>
        </div>
    );
}