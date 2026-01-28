const { contextBridge, ipcRenderer, app } = require('electron');
import type { Event } from 'electron';
import { AudioCaptureOptions, CueParams, UploadBothFilesOptions, UploadFileOptions } from './globals';

try {
  contextBridge.exposeInMainWorld('electron', {
    ipcRenderer: {
      invoke: (channel: string, ...args: unknown[]) => {
        return ipcRenderer.invoke(channel, ...args);
      },
      on: (channel: string, callback: (data: unknown) => void) => {
        console.log('🔌 PRELOAD: Setting up listener for channel:', channel);
        ipcRenderer.on(channel, (event: Event, ...args: unknown[]) => {
          console.log('📨 PRELOAD: Received data on channel:', channel, args);
          callback(args[0]);
        });
        return () => {
          console.log('🧹 PRELOAD: Cleaning up listener for channel:', channel);
          ipcRenderer.removeAllListeners(channel);
        };
      },
      send: (channel: string, ...args: unknown[]) => {
        ipcRenderer.send(channel, ...args);
      },
      removeAllListeners: (channel: string) => {
        ipcRenderer.removeAllListeners(channel);
      },
      off: (channel: string, callback: (...args: unknown[]) => void) => {
        console.log('🧹 PRELOAD: Removing listener for channel:', channel);
        ipcRenderer.removeListener(channel, callback);
      }
    },
    // Add openExternal method for opening URLs in external browser
    openExternal: (url: string) => {
      ipcRenderer.send('open-external', url);
    },
    
    // Native Audio Module API
    nativeAudio: {
      initialize: () => ipcRenderer.invoke('native-audio-initialize'),
      listDevices: () => ipcRenderer.invoke('native-audio-list-devices'),
      createDevice: (name: string, subDevices: string[]) => ipcRenderer.invoke('native-audio-create-device', { name, subDevices }),
      deleteDevice: (deviceId: string) => ipcRenderer.invoke('native-audio-delete-device', { deviceId }),
      requestPermission: () => ipcRenderer.invoke('native-audio-request-permission'),
      startCapture: (options: AudioCaptureOptions) => ipcRenderer.invoke('native-audio-start-capture', options),
      stopCapture: () => ipcRenderer.invoke('native-audio-stop-capture'),
      isCapturing: () => ipcRenderer.invoke('native-audio-is-capturing')
    },
    
    // Dual Channel Recording API
    recording: {
      startDualChannel: (params: AudioCaptureOptions) => ipcRenderer.invoke('start-audio-capture', params),
      stopDualChannel: () => ipcRenderer.invoke('stop-audio-capture'),
      compressAudio: (options: AudioCaptureOptions) => ipcRenderer.invoke('compress-audio', options)
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
      }
    },
    
    // File Upload API
    uploadFile: (options: UploadFileOptions) => ipcRenderer.invoke('upload-file', { ...options }),
    uploadBothFiles: (options: UploadBothFilesOptions) => ipcRenderer.invoke('upload-both-files', { ...options }),

    // Permissions API
    permissions: {
      check: () => ipcRenderer.invoke('permissions-check'),
      requestAll: () => ipcRenderer.invoke('permissions-request-all')
    }
  });

  contextBridge.exposeInMainWorld('electronAPI', {
    resizeWindow: (width: number, height: number) => ipcRenderer.send('resize-coach-window', width, height),
    closeCoachWindow: () => ipcRenderer.send('close-coach-window'),
    getWindowPosition: () => ipcRenderer.invoke('get-window-position'),
    setWindowPosition: (x: number, y: number) => ipcRenderer.send('set-window-position', x, y)
  });

  // Extract --indexHtmlPath from process.argv
  const indexHtmlPathArg = process.argv.find(arg => arg.startsWith('--indexHtmlPath='));
  const indexHtmlPath = indexHtmlPathArg ? indexHtmlPathArg.replace('--indexHtmlPath=', '') : '';

  contextBridge.exposeInMainWorld('sayso', {
    indexHtmlPath
  });


} catch (error) {
  console.error('Preload Error: Failed to exposeInMainWorld:', error);
}


