import { contextBridge, ipcRenderer, app } from 'electron';
import type { Event } from 'electron';
import { CueParams } from './globals';

try {
  if (document.documentElement) {
    document.documentElement.dataset.arch = process.arch;
  } else {
    document.addEventListener('DOMContentLoaded', () => {
      document.documentElement.dataset.arch = process.arch;
    });
  }

  contextBridge.exposeInMainWorld('electron', {
    ipcRenderer: {
      invoke: (channel: string, ...args: unknown[]) => {
        return ipcRenderer.invoke(channel, ...args);
      },
      on: (channel: string, callback: (data: unknown) => void) => {
        const listener = (_event: Event, ...args: unknown[]) => {
          callback(args[0]);
        };
        ipcRenderer.on(channel, listener);
        // Remove only this listener on cleanup, so other components on the same channel are unaffected.
        return () => {
          ipcRenderer.removeListener(channel, listener);
        };
      },
      send: (channel: string, ...args: unknown[]) => {
        ipcRenderer.send(channel, ...args);
      },
      removeAllListeners: (channel: string) => {
        ipcRenderer.removeAllListeners(channel);
      }
      // No off(): `on` above registers an internal wrapper, so removeListener
      // could never match the caller's callback — it was broken by construction
      // at every call site, silently leaking listeners. Use the disposer that
      // `on` returns instead. SAYSO-338.
    },
    // Add openExternal method for opening URLs in external browser
    openExternal: (url: string) => {
      ipcRenderer.send('open-external', url);
    },

    // Cue API (handles 2 audio websockets + insights websocket)
    cue: {
      start: (params: CueParams) => ipcRenderer.invoke('start-cue', params),
      stop: () => ipcRenderer.invoke('stop-cue'),
      // Listen for Cue status updates
      onStatus: (callback: (data: any) => void) => {
        ipcRenderer.on('cue-status', (event: Event, data: any) => callback(data));
        return () => ipcRenderer.removeAllListeners('cue-status');
      },
      // Listen for insights (TODO: will be implemented when insights websocket is added)
      onInsight: (callback: (data: any) => void) => {
        ipcRenderer.on('cue-insight', (event: Event, data: any) => callback(data));
        return () => ipcRenderer.removeAllListeners('cue-insight');
      },
      // Listen for Cue errors
      onError: (callback: (data: any) => void) => {
        ipcRenderer.on('cue-error', (event: Event, data: any) => callback(data));
        return () => ipcRenderer.removeAllListeners('cue-error');
      },
      onAutoStop: (callback: (data: any) => void) => {
        ipcRenderer.on('cue-auto-stop', (event: Event, data: any) => callback(data));
        return () => ipcRenderer.removeAllListeners('cue-auto-stop');
      },
      onLowUserAudio: (callback: (data: { sessionId: string }) => void) => {
        ipcRenderer.on('cue-low-user-audio', (_event: Event, data: { sessionId: string }) => callback(data));
        return () => ipcRenderer.removeAllListeners('cue-low-user-audio');
      },
      onMicRecoveryFailed: (callback: () => void) => {
        ipcRenderer.on('cue-mic-recovery-failed', () => callback());
        return () => ipcRenderer.removeAllListeners('cue-mic-recovery-failed');
      },
      onMicRecoverySucceeded: (callback: () => void) => {
        ipcRenderer.on('cue-mic-recovery-succeeded', () => callback());
        return () => ipcRenderer.removeAllListeners('cue-mic-recovery-succeeded');
      },
      onSmartCapture: (callback: (data: { topic: string; content: string }[]) => void) => {
        ipcRenderer.on('cue-smart-capture', (_event: Event, data: { topic: string; content: string }[]) => callback(data));
        return () => ipcRenderer.removeAllListeners('cue-smart-capture');
      }
    },
    
    // Permissions API
    permissions: {
      check: () => ipcRenderer.invoke('permissions-check'),
      requestMic: () => ipcRenderer.invoke('permissions-request-mic'),
      checkScreen: () => ipcRenderer.invoke('permissions-check-screen'),
      requestScreen: () => ipcRenderer.invoke('permissions-request-screen'),
      openScreenSettings: () => ipcRenderer.invoke('permissions-open-screen-settings'),
      complete: () => ipcRenderer.invoke('permissions-complete'),
      getFlag: () => ipcRenderer.invoke('permissions-get-flag'),
    },
    
    autoUpdater: {
        onUpdateCheckComplete: (callback: () => void) => {
            ipcRenderer.on('update-check-complete', () => callback());
            return () => ipcRenderer.removeAllListeners('update-check-complete');
        },
    },

    update: {
        getState: () => ipcRenderer.invoke('update:get-state'),
        onStateChanged: (callback: (state: any) => void) => {
            ipcRenderer.on('update:state-changed', (_event: Event, data: any) => callback(data));
            return () => ipcRenderer.removeAllListeners('update:state-changed');
        },
        startDownload: () => ipcRenderer.send('update:start-download'),
        dismiss: () => ipcRenderer.send('update:dismiss'),
        checkForUpdates: () => ipcRenderer.send('update:check-for-updates'),
    },

    app: {
        getVersion: () => ipcRenderer.invoke('app:get-version'),
    },

    appSettings: {
        openUpdateTab: () => ipcRenderer.send('app-settings:open-update-tab'),
    },
  });

  contextBridge.exposeInMainWorld('electronAPI', {
    resizeWindow: (width: number, height: number) => ipcRenderer.send('resize-coach-window', width, height),
    closeCoachWindow: () => ipcRenderer.send('close-coach-window'),
    getCoachWorkAreaBottom: (): Promise<number | null> => ipcRenderer.invoke('get-coach-work-area-bottom'),
    getWindowPosition: () => ipcRenderer.invoke('get-window-position'),
    setWindowPosition: (x: number, y: number) => ipcRenderer.send('set-window-position', x, y),
    playbook: {
      getWindowPosition: () => ipcRenderer.invoke('get-playbook-window-position'),
      setWindowPosition: (x: number, y: number) => ipcRenderer.send('set-playbook-window-position', x, y)
    }
  });

  // Extract --indexHtmlPath from process.argv
  const indexHtmlPathArg = process.argv.find(arg => arg.startsWith('--indexHtmlPath='));
  const indexHtmlPath = indexHtmlPathArg ? indexHtmlPathArg.replace('--indexHtmlPath=', '') : '';

  contextBridge.exposeInMainWorld('sayso', {
    indexHtmlPath,
    platform: process.platform
  });


} catch (error) {
  console.error('Preload Error: Failed to exposeInMainWorld:', error);
}


