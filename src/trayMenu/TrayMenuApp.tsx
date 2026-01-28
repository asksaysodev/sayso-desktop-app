import React, { useEffect, useMemo, useState } from 'react';
import trayToggleOn from '/assets/tray-toggle-on.png';
import trayToggleOff from '/assets/tray-toggle-off.png';
import { Account } from '@/types/user';

/**
 * Tray Menu App - Custom menu window for system tray
 * Replaces native Electron tray menu with custom BrowserWindow for better control
 */
const TrayMenuApp = () => {
  const [isCoachOpen, setIsCoachOpen] = useState(false);
  const [userAuthenticated, setUserAuthenticated] = useState<Account | null>(null);

  const disableToggleCoach = useMemo(() => {
    return !userAuthenticated || userAuthenticated?.subscription_plan_id === null;
  }, [userAuthenticated]);

  useEffect(() => {
    const ipcRenderer = window.electron?.ipcRenderer;
    
    if (!ipcRenderer) return;

    const handleUserAuth = (state: { authUser: Account | null }) => {
      setUserAuthenticated(state.authUser);
    };

    const handleCoachWindowState = (state: { isOpen: boolean }) => {
      setIsCoachOpen(state.isOpen);
    };

    ipcRenderer.on('coach-window-state', handleCoachWindowState as any); // $FixTS
    ipcRenderer.send('get-coach-window-state');

    ipcRenderer.on('user-auth', handleUserAuth as any); // $FixTS
    ipcRenderer.send('get-user-auth');

    return () => {
      ipcRenderer.off('coach-window-state', handleCoachWindowState as any); // $FixTS
      ipcRenderer.off('user-auth', handleUserAuth as any); // $FixTS
    };
  }, []);

  const handleToggleCoach = () => {
    const ipcRenderer = window.electron?.ipcRenderer;

    if (ipcRenderer) {
      if (isCoachOpen) {
        ipcRenderer.send('close-coach-window'); 
      } else {
        ipcRenderer.send('open-coach-window');
      }
    }
  };

  const handleQuit = () => {
    const ipcRenderer = window.electron?.ipcRenderer;

    if (ipcRenderer) {
      ipcRenderer.send('quit-app');
    }
  };

  return (
    <div className="tray-menu">
      <div className="tray-menu-header">
        <span className="tray-menu-title">Sayso</span>
      </div>
      
      <div className="tray-menu-items">
        <button 
          className="tray-menu-item"
          onClick={handleToggleCoach}
          disabled={disableToggleCoach}
        >
          <div className="tray-menu-item-icon">
            <img 
              src={isCoachOpen ? trayToggleOn : trayToggleOff}
              alt={isCoachOpen ? 'Close' : 'Open'}
              className="tray-menu-item-icon-img"
            />
          </div>
          <span className="tray-menu-item-label">
            {isCoachOpen ? 'Close Coach' : 'Start Coach'}
          </span>
        </button>

        <div className="tray-menu-separator" />

        <button 
          className="tray-menu-item"
          onClick={handleQuit}
        >
          <span className="tray-menu-item-label">Quit</span>
        </button>
      </div>
    </div>
  );
};

export default TrayMenuApp;

