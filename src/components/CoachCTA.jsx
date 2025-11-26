import {LuRocket} from 'react-icons/lu'

import '../styles/CoachCTA.css';
import { useCoachWindowContext } from '../context/CoachWindowContext';

export default function CoachCTA({sidebar, active }) {
    const { isCoachWindowOpen, closeCoachWindow, openCoachWindow } = useCoachWindowContext();

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