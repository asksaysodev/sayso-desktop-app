import {LuRocket} from 'react-icons/lu'

import '../styles/CoachCTA.css';
import { useCoachWindowStore } from '../store/coachWindowStore';
import { useEffect, useState } from 'react';

export default function CoachCTA({sidebar, active }) {
    const [isCoachWindowOpen, setIsCoachWindowOpen] = useState(false);
    const openCoachWindow = useCoachWindowStore(state => state.openCoachWindow);
    const closeCoachWindow = useCoachWindowStore(state => state.closeCoachWindow);

    useEffect(() => {
        const ipcRenderer = window.electron?.ipcRenderer;

        const syncState = async () => {
            if (ipcRenderer) {
                try {
                    const isOpen = await ipcRenderer.invoke('get-coach-window-open-state');
                    setIsCoachWindowOpen(isOpen);
                } catch (error) {
                    console.error('Error checking coach window state:', error);
                    setIsCoachWindowOpen(false);
                }
            }
        };

        syncState();

        if (ipcRenderer) {
            const handleCoachWindowClosed = () => {
                setIsCoachWindowOpen(false);
            };

            const handleCoachWindowOpened = () => {
                setIsCoachWindowOpen(true);
            };

            try {
                ipcRenderer.on('coach-window-closed', handleCoachWindowClosed);
                ipcRenderer.on('coach-window-opened', handleCoachWindowOpened);
            } catch (error) {
                console.error('Error setting up coach window listeners:', error);
            }

            return () => {
                try {
                    ipcRenderer.removeListener('coach-window-closed', handleCoachWindowClosed);
                    ipcRenderer.removeListener('coach-window-opened', handleCoachWindowOpened);
                } catch (error) {
                    console.error('Error removing coach window listeners:', error);
                }
            };
        }
    }, []);


    async function handleOnPressStartCoach() {
        if (isCoachWindowOpen) {
            closeCoachWindow();
        } else {
            try {
                await openCoachWindow();
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