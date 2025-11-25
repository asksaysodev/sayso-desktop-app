import { useState, useEffect } from 'react';

import {LuRocket} from 'react-icons/lu'

import '../styles/CoachCTA.css';

export default function CoachCTA({sidebar, active }) {

  const [isCoachWindowOpen, setIsCoachWindowOpen] = useState(() => {
    // Initialize from localStorage
    return localStorage.getItem('coach-window-open') === 'true';
  });

  /**
   * @param {boolean} isOpen
   */
  function updateCoachWindowOpenStates(isOpen) {
    setIsCoachWindowOpen(isOpen);
    localStorage.setItem('coach-window-open', String(isOpen));
  }
  
  // Listen for localStorage changes
  useEffect(() => {
    const handleStorageChange = (e) => {
      if (e.key === 'coach-window-open') {
        const isOpen = e.newValue === 'true';
        console.log(' [CoachCTA] localStorage changed, updating state:', isOpen);
        updateCoachWindowOpenStates(isOpen)
      }
    };

    // Listen for storage events (when localStorage changes in other tabs/contexts)
    window.addEventListener('storage', handleStorageChange);

    // Also listen for the coach-window-closed event as a backup
    if (window.electron && window.electron.ipcRenderer) {
      const handleCoachWindowClosed = () => {
        console.log('🎯 [CoachCTA] Coach window closed event received');
        updateCoachWindowOpenStates(false)
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
      const tryOpen = () => {
          if (window.electron && window.electron.ipcRenderer) {
              window.electron.ipcRenderer.send('open-coach-window');
              updateCoachWindowOpenStates(true)
              return true;
          }
          return false;
      };
      
      if (!tryOpen()) {
          console.log('⏳ [CoachCTA] Electron not available yet, waiting...');
          const checkInterval = setInterval(() => {
              if (tryOpen()) {
                  clearInterval(checkInterval);
                  console.log('✅ [CoachCTA] Electron available, coach window opened');
              }
          }, 50);
          
          setTimeout(() => {
              clearInterval(checkInterval);
              if (!window.electron || !window.electron.ipcRenderer) {
                  console.error('❌ [CoachCTA] Electron still not available after 1 second');
              }
          }, 1000);
      }
  };

  const closeCoachWindow = () => {
    if (window.electron && window.electron.ipcRenderer) {
      window.electron.ipcRenderer.send('close-coach-window');
      updateCoachWindowOpenStates(false)
    }
  };

  function handleOnPressStartCoach() {
    if (isCoachWindowOpen) {
      closeCoachWindow()
    } else {
      openCoachWindow()
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