import React, { useEffect, useMemo, useRef, useState } from 'react';
import * as Sentry from '@sentry/electron/renderer';
import { Account } from '@/types/user';
import { ExternalLink } from 'lucide-react';
import { useAppSettingsWindow } from '@/hooks/useAppSettingsWindow';
import { usePlaybookWindow } from '@/hooks/usePlaybookWindow';
import { useEnabledFeatures } from '@/hooks/useEnabledFeatures';
import { useNetworkState } from '@/hooks/useNetworkState';
import { UpdatePhase } from '@/types/update';

const TrayMenuApp = () => {
  const [isCoachOpen, setIsCoachOpen] = useState(false);
  const [isAuthenticated, setIsAuthenticated] = useState(false);
  const [account, setAccount] = useState<Account | null>(null);
  const [updatePhase, setUpdatePhase] = useState<UpdatePhase>('idle');
  const { isReconnecting } = useNetworkState();
  const { toggleAppSettingsWindow } = useAppSettingsWindow();
  const { isPlaybookWindowOpen, togglePlaybookWindow } = usePlaybookWindow();
  const { hasFeature } = useEnabledFeatures();

  const disableToggleCoach = useMemo(() => {
    return !isAuthenticated || !account || account.subscription_plan_id === null;
  }, [isAuthenticated, account]);

  const showPlaybooksRow = useMemo(() => {
    return isAuthenticated && !!account && account.subscription_plan_id !== null && hasFeature('playbooks');
  }, [isAuthenticated, account, hasFeature]);

  const isUpdating: boolean = updatePhase === 'downloading' || updatePhase === 'downloaded';
  const isBlocked: boolean = updatePhase === 'blocked';
  const showUpdateRow: boolean = updatePhase === 'available' || isBlocked || isUpdating;

  useEffect(() => {
    const ipcRenderer = window.electron?.ipcRenderer;
    if (!ipcRenderer) return;

    const handleUserAuth = (state: { authUser: Account | null }) => {
      setAccount(state.authUser || null);
    };

    // The auth:get-state reply is computed in main before a concurrent sign-out
    // broadcast is emitted, so a late reply can flip us back to authenticated
    // after the event correctly cleared it. Once an event has been seen, it wins.
    let sawAuthStateEvent = false;

    const handleAuthState = (state: { isAuthenticated?: boolean }) => {
      sawAuthStateEvent = true;
      setIsAuthenticated(!!state?.isAuthenticated);
    };

    const handleCoachWindowState = (state: { isOpen: boolean }) => {
      setIsCoachOpen(state.isOpen);
    };

    const offCoachWindowState = ipcRenderer.on('coach-window-state', handleCoachWindowState as any);
    ipcRenderer.send('get-coach-window-state');

    const offUserAuth = ipcRenderer.on('user-auth', handleUserAuth as any);
    ipcRenderer.send('get-user-auth');

    const offAuthState = ipcRenderer.on('auth:state', handleAuthState as any);
    ipcRenderer.invoke('auth:get-state')
      .then((state: any) => {
        if (sawAuthStateEvent) return;
        setIsAuthenticated(!!state?.isAuthenticated);
      })
      .catch(Sentry.captureException);

    return () => {
      offCoachWindowState?.();
      offUserAuth?.();
      offAuthState?.();
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

  const contentRef = useRef<HTMLDivElement>(null);
  const lastSentHeight = useRef<number | null>(null);

  const reportHeight = () => {
    const el = contentRef.current;
    const ipcRenderer = window.electron?.ipcRenderer;
    if (!el || !ipcRenderer) return;
    const height = Math.ceil(el.getBoundingClientRect().height);
    if (!height || height === lastSentHeight.current) return;
    lastSentHeight.current = height;
    ipcRenderer.send('set-tray-menu-height', height);
  };

  // No dep array on purpose, and no ResizeObserver: a `show: false` window runs
  // no rendering pipeline, so RO notifications are throttled while direct layout
  // reads are not — and this window is hidden almost all of its life.
  useEffect(reportHeight);

  useEffect(() => {
    document.addEventListener('visibilitychange', reportHeight);
    return () => document.removeEventListener('visibilitychange', reportHeight);
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
    if (isAuthenticated) {
      ipc.send('tray-logout');
    } else {
      ipc.send('tray-show-window');
    }
  };

  const handleOpenUpdateTab = () => {
    window.electron?.appSettings?.openUpdateTab();
  };

  const handleOpenHelpCenter = () => {
    window.electron?.openExternal('https://asksayso.notion.site/helpcenter');
  };

  return (
    <div className="tray-menu">
      <div className="tray-menu-items" ref={contentRef}>
        {!disableToggleCoach && (
          <>
            <button
              className="tray-menu-item"
              onClick={handleToggleCoach}
              disabled={disableToggleCoach || isUpdating || isReconnecting}
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
              disabled={isUpdating || isReconnecting}
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

        {isAuthenticated && (
          <>
            <button
              className="tray-menu-item"
              onClick={toggleAppSettingsWindow}
              disabled={isUpdating || isReconnecting}
            >
              <span className="tray-menu-item-label">Settings</span>
            </button>

            <div className="tray-menu-separator" />

            <button
              className="tray-menu-item"
              onClick={handlePressMyAccount}
            >
              <span className="tray-menu-item-label">My Account</span>
              <ExternalLink size={16} />
            </button>

            <div className="tray-menu-separator" />
          </>
        )}

        {showUpdateRow && (
          <>
            <button
              className="tray-menu-item"
              onClick={isUpdating || isReconnecting || isBlocked ? undefined : handleOpenUpdateTab}
              disabled={isUpdating || isReconnecting || isBlocked}
            >
              {updatePhase === 'available' && <span className="tray-update-dot" />}
              <span className="tray-menu-item-label">
                {isBlocked ? 'Move to Applications Folder' : isUpdating ? 'Update in progress…' : 'Update Available'}
              </span>
            </button>
            <div className="tray-menu-separator" />
          </>
        )}

        <button className="tray-menu-item" onClick={handleOpenHelpCenter}>
          <span className="tray-menu-item-label">Help Center</span>
          <ExternalLink size={16} />
        </button>
        <div className="tray-menu-separator" />

        <div className="tray-menu-bottom-row">
          <button className="tray-menu-item tray-menu-item--bottom" onClick={handleQuit}>
            <span className="tray-menu-item-label bottom-item">Quit</span>
          </button>
          <button
            className="tray-menu-item tray-menu-item--bottom"
            onClick={handleAuthPress}
          >
            <span className="tray-menu-item-label bottom-item">
              {isAuthenticated ? 'Log Out' : 'Log In'}
            </span>
          </button>
        </div>
      </div>
    </div>
  );
};

export default TrayMenuApp;
