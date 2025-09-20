import { useState, useEffect } from 'react';

import {LuRocket} from 'react-icons/lu'

import '../styles/CoachCTA.css';

export default function CoachCTA({sidebar, active }) {

  const [isCoachWindowOpen, setIsCoachWindowOpen] = useState(() => {
    // Initialize from localStorage
    return localStorage.getItem('coach-window-open') === 'true';
  });

  // Listen for localStorage changes
  useEffect(() => {
    const handleStorageChange = (e) => {
      if (e.key === 'coach-window-open') {
        const isOpen = e.newValue === 'true';
        console.log(' [CoachCTA] localStorage changed, updating state:', isOpen);
        setIsCoachWindowOpen(isOpen);
      }
    };

    // Listen for storage events (when localStorage changes in other tabs/contexts)
    window.addEventListener('storage', handleStorageChange);

    // Also listen for the coach-window-closed event as a backup
    if (window.electron && window.electron.ipcRenderer) {
      const handleCoachWindowClosed = () => {
        console.log('🎯 [CoachCTA] Coach window closed event received');
        setIsCoachWindowOpen(false);
      };

      window.electron.ipcRenderer.on('coach-window-closed', handleCoachWindowClosed);

      return () => {
        window.electron.ipcRenderer.removeAllListeners('coach-window-closed');
        window.removeEventListener('storage', handleStorageChange);
      };
    }

    return () => {
      window.removeEventListener('storage', handleStorageChange);
    };
  }, []);

  const openCoachWindow = () => {
      if (window.electron && window.electron.ipcRenderer) {
          window.electron.ipcRenderer.send('open-coach-window');
          setIsCoachWindowOpen(true);
          localStorage.setItem('coach-window-open', 'true');
      } else {
          console.warn('Electron not available, cannot open coach window');
          setIsCoachWindowOpen(false);
      }
  };

  const closeCoachWindow = () => {
      if (window.electron && window.electron.ipcRenderer) {
          window.electron.ipcRenderer.send('close-coach-window');
          setIsCoachWindowOpen(false);
          localStorage.setItem('coach-window-open', 'false');
      }
  };

  return (
    <div className={`coach-cta-container ${sidebar ? 'sidebar' : ''} ${active ? 'active' : ''}`} onClick={isCoachWindowOpen ? closeCoachWindow : openCoachWindow}>
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