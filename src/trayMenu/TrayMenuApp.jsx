import React, { useEffect, useState } from 'react';
import trayToggleOn from '/assets/tray-toggle-on.png';
import trayToggleOff from '/assets/tray-toggle-off.png';

/**
 * Tray Menu App - Custom menu window for system tray
 * Replaces native Electron tray menu with custom BrowserWindow for better control
 */
const TrayMenuApp = () => {
  const [isCoachOpen, setIsCoachOpen] = useState(false);

  useEffect(() => {
    const ipcRenderer = window.electron?.ipcRenderer;
    
    if (!ipcRenderer) return;

    const handleCoachWindowState = (state) => {
      setIsCoachOpen(state.isOpen);
    };

    ipcRenderer.on('coach-window-state', handleCoachWindowState);
    ipcRenderer.send('get-coach-window-state');

    return () => {
      ipcRenderer.off('coach-window-state', handleCoachWindowState);
    };
  }, []);

  const handleToggleCoach = () => {
    if (window.electron?.ipcRenderer) {
      if (isCoachOpen) {
        window.electron.ipcRenderer.send('close-coach-window'); 
      } else {
        window.electron.ipcRenderer.send('open-coach-window');
      }
    }
  };

  const handleQuit = () => {
    if (window.electron?.ipcRenderer) {
      window.electron.ipcRenderer.send('quit-app');
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

