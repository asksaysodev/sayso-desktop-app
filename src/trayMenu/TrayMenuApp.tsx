import React, { useEffect, useMemo, useState } from 'react';
import * as Sentry from '@sentry/electron/renderer';
import { Account } from '@/types/user';
import { ExternalLink } from 'lucide-react';
import { useAppSettingsWindow } from '@/hooks/useAppSettingsWindow';
import { usePlaybookWindow } from '@/hooks/usePlaybookWindow';
import { useEnabledFeatures } from '@/hooks/useEnabledFeatures';
import { UpdatePhase } from '@/types/update';

const TrayMenuApp = () => {
  const [isCoachOpen, setIsCoachOpen] = useState(false);
  const [userAuthenticated, setUserAuthenticated] = useState<Account | null>(null);
  const [updatePhase, setUpdatePhase] = useState<UpdatePhase>('idle');
  const { toggleAppSettingsWindow } = useAppSettingsWindow();
  const { isPlaybookWindowOpen, togglePlaybookWindow } = usePlaybookWindow();
  const { hasFeature } = useEnabledFeatures();

  const disableToggleCoach = useMemo(() => {
    return !userAuthenticated || userAuthenticated?.subscription_plan_id === null;
  }, [userAuthenticated]);

  const showPlaybooksRow = useMemo(() => {
    return userAuthenticated && userAuthenticated?.subscription_plan_id !== null && hasFeature('playbooks');
  }, [userAuthenticated, hasFeature]);

  const isUpdating: boolean = updatePhase === 'downloading' || updatePhase === 'downloaded';
  const showUpdateRow: boolean = updatePhase === 'available' || isUpdating;

  useEffect(() => {
    const ipcRenderer = window.electron?.ipcRenderer;
    if (!ipcRenderer) return;

    const handleUserAuth = (state: { authUser: Account | null }) => {
      setUserAuthenticated(state.authUser);
    };

    const handleCoachWindowState = (state: { isOpen: boolean }) => {
      setIsCoachOpen(state.isOpen);
    };

    ipcRenderer.on('coach-window-state', handleCoachWindowState as any);
    ipcRenderer.send('get-coach-window-state');

    ipcRenderer.on('user-auth', handleUserAuth as any);
    ipcRenderer.send('get-user-auth');

    return () => {
      ipcRenderer.off('coach-window-state', handleCoachWindowState as any);
      ipcRenderer.off('user-auth', handleUserAuth as any);
    };
  }, []);

  // Subscribe to update state
  useEffect(() => {
    const cleanup = window.electron?.update?.onStateChanged((state) => {
      setUpdatePhase(state.phase);
    });
    window.electron?.update?.getState().then((state) => {
      setUpdatePhase(state.phase);
    }).catch(Sentry.captureException);
    return cleanup;
  }, []);

  useEffect(() => {
    const ipcRenderer = window.electron?.ipcRenderer;
    if (!ipcRenderer) return;

    // Base height: Quit/Log In bottom row (42) + bottom padding
    // Each row is ~42px. Logged-out: just bottom row = 46px.
    let height: number;
    if (!userAuthenticated) {
      height = 46;
    } else if (disableToggleCoach) {
      // My Account + Settings + bottom row
      height = 46 + 42 * 2;
    } else {
      // Coach + My Account + Settings + bottom row
      height = 46 + 42 * 3;
    }
    if (showPlaybooksRow) height += 42;
    if (showUpdateRow) height += 42;

    ipcRenderer.send('set-tray-menu-height', height);
  }, [disableToggleCoach, userAuthenticated, showPlaybooksRow, showUpdateRow]);

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
    window.electron?.ipcRenderer?.send('quit-app');
  };

  const handlePressMyAccount = async () => {
    const token: string | null = await window.electron?.ipcRenderer?.invoke('auth:get-token') ?? null;
    const url = new URL('https://app.asksayso.com/settings');
    if (token) url.hash = `access_token=${token}`;
    window.electron?.openExternal(url.toString());
  };

  const handleAuthPress = () => {
    const ipc = window.electron?.ipcRenderer;
    if (!ipc) return;
    if (userAuthenticated) {
      ipc.send('tray-logout');
    } else {
      ipc.send('tray-show-window');
    }
  };

  const handleOpenUpdateTab = () => {
    window.electron?.appSettings?.openUpdateTab();
  };

  return (
    <div className="tray-menu">
      <div className="tray-menu-items">
        {!disableToggleCoach && (
          <>
            <button
              className="tray-menu-item"
              onClick={handleToggleCoach}
              disabled={disableToggleCoach || isUpdating}
            >
              <span className="tray-menu-item-label">
                {isCoachOpen ? 'Close Coach' : 'Launch Coach'}
              </span>
              <span className="tray-menu-shortcut">
                Ctrl + S
              </span>
            </button>
            <div className="tray-menu-separator" />
          </>
        )}

        {showPlaybooksRow && (
          <>
            <button
              className="tray-menu-item"
              onClick={togglePlaybookWindow}
              disabled={isUpdating}
            >
              <span className="tray-menu-item-label">
                {isPlaybookWindowOpen ? 'Hide Playbooks' : 'Show Playbooks'}
              </span>
              <span className="tray-menu-shortcut">
                Ctrl + B
              </span>
            </button>
            <div className="tray-menu-separator" />
          </>
        )}

        {userAuthenticated && (
          <>
            <button
              className="tray-menu-item"
              onClick={handlePressMyAccount}
            >
              <span className="tray-menu-item-label">My Account</span>
              <ExternalLink size={16} />
            </button>

            <div className="tray-menu-separator" />

            <button
              className="tray-menu-item"
              onClick={toggleAppSettingsWindow}
              disabled={isUpdating}
            >
              <span className="tray-menu-item-label">Settings</span>
            </button>

            <div className="tray-menu-separator" />
          </>
        )}

        {showUpdateRow && (
          <>
            <button
              className="tray-menu-item"
              onClick={isUpdating ? undefined : handleOpenUpdateTab}
              disabled={isUpdating}
            >
              {updatePhase === 'available' && <span className="tray-update-dot" />}
              <span className="tray-menu-item-label">
                {isUpdating ? 'Update in progress…' : 'Update Available'}
              </span>
            </button>
            <div className="tray-menu-separator" />
          </>
        )}

        <div className="tray-menu-bottom-row">
          <button className="tray-menu-item tray-menu-item--bottom" onClick={handleQuit}>
            <span className="tray-menu-item-label bottom-item">Quit</span>
          </button>
          <button
            className="tray-menu-item tray-menu-item--bottom"
            onClick={handleAuthPress}
          >
            <span className="tray-menu-item-label bottom-item">
              {userAuthenticated ? 'Log Out' : 'Log In'}
            </span>
          </button>
        </div>
      </div>
    </div>
  );
};

export default TrayMenuApp;
