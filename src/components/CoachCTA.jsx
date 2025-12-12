import {LuRocket} from 'react-icons/lu'

import '../styles/CoachCTA.css';
import { useCoachWindowStore } from '../store/coachWindowStore';

export default function CoachCTA({sidebar, active }) {
    const openCoachWindow = useCoachWindowStore(state => state.openCoachWindow);
    const closeCoachWindow = useCoachWindowStore(state => state.closeCoachWindow);

    async function handleOnPressCoachButton() {
        const ipcRenderer = window.electron?.ipcRenderer;
        if (!ipcRenderer) return;
        
        const isOpen = await ipcRenderer.invoke('get-coach-window-open-state');

        if (isOpen) {
            closeCoachWindow();
        } else {
            try {
                await openCoachWindow();
            } catch (error) {
                console.error('Error opening coach window:', error);
            }
        }
    }

    return (
        <div
            className={`coach-cta-container ${sidebar ? 'sidebar' : ''} ${active ? 'active' : ''}`}
            onClick={handleOnPressCoachButton}
        >
            {
                !sidebar && (
                    <div className="outline"></div>
                )
            }
            <div className="main-button">
				<div className="text-container">
					<p>{'Start Coach' }</p>
					<span>Ctrl + S</span>
				</div>
            </div>
        </div>
    );
}
