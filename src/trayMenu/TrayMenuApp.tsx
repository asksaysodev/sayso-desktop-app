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
console.log(userAuthenticated,'dasdadsa')
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

  useEffect(() => {
    const ipcRenderer = window.electron?.ipcRenderer;
    if (!ipcRenderer) return;
      let height;
      if (disableToggleCoach && !userAuthenticated) {
          height = 82;
      } else if (!disableToggleCoach && userAuthenticated) {
          height = 172;
      } else {
          height = 128;
      }
      
    ipcRenderer.send('set-tray-menu-height', height);
  }, [disableToggleCoach, userAuthenticated]);

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
    
    const handlePressAccountSettings = async () => {
        const ipc = window.electron?.ipcRenderer;
        if (!ipc) return;
        const { accessToken, refreshToken } = await ipc.invoke('get-auth-tokens') as { accessToken: string | null; refreshToken: string | null };
        const url = new URL('https://app.asksayso.com/settings');
         if (accessToken) url.hash = `access_token=${accessToken}&refresh_token=${refreshToken}`;                                                                               
         window.electron?.openExternal(url.toString());
    }
    const handleAuthPress = () => {
        const ipc = window.electron?.ipcRenderer;
        if (!ipc) return;
        if (userAuthenticated) {
            ipc.send('tray-logout');
        } else {
            ipc.send('tray-show-window');
        }
    }
    
  return (
    <div className="tray-menu">
      <div className="tray-menu-header">
        <span className="tray-menu-title">Sayso</span>
      </div>
      
      <div className="tray-menu-items">
        {!disableToggleCoach && (
          <>
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
                {isCoachOpen ? 'Close coach' : 'Start coach'}
              </span>
            </button>
            <div className="tray-menu-separator" />
          </>
        )}
        
        {userAuthenticated && 
            <>
                <button
                    className="tray-menu-item"
                    onClick={handlePressAccountSettings}
                >
                    <span className="tray-menu-item-label">
                        Account settings
                    </span>
                </button>
                
                <div className="tray-menu-separator" />
            </>
        }
        

        <div className="tray-menu-bottom-row">
          <button className="tray-menu-item tray-menu-item--bottom" onClick={handleQuit}>
            <span className="tray-menu-item-label bottom-item">Quit</span>
          </button>
          <button
            className="tray-menu-item tray-menu-item--bottom"
            onClick={handleAuthPress}
          >
            <span className="tray-menu-item-label bottom-item">
              {userAuthenticated ? 'Log out' : 'Log in'}
            </span>
          </button>
        </div>
      </div>
    </div>
  );
};

export default TrayMenuApp;

