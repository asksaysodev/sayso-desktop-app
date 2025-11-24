const { contextBridge, ipcRenderer, app } = require('electron');


try {
  contextBridge.exposeInMainWorld('electron', {
    ipcRenderer: {
      invoke: (channel, ...args) => {
        return ipcRenderer.invoke(channel, ...args);
      },
      on: (channel, callback) => {
        console.log('🔌 PRELOAD: Setting up listener for channel:', channel);
        ipcRenderer.on(channel, (event, ...args) => {
          console.log('📨 PRELOAD: Received data on channel:', channel, args);
          callback(args[0]);
        });
        return () => {
          console.log('🧹 PRELOAD: Cleaning up listener for channel:', channel);
          ipcRenderer.removeAllListeners(channel);
        };
      },
      send: (channel, ...args) => {
        ipcRenderer.send(channel, ...args);
      },
      removeAllListeners: (channel) => {
        ipcRenderer.removeAllListeners(channel);
      },
      off: (channel, callback) => {
        console.log('🧹 PRELOAD: Removing listener for channel:', channel);
        ipcRenderer.removeListener(channel, callback);
      }
    },
    // Add openExternal method for opening URLs in external browser
    openExternal: (url) => {
      ipcRenderer.send('open-external', url);
    },
    
    // Native Audio Module API
    nativeAudio: {
      initialize: () => ipcRenderer.invoke('native-audio-initialize'),
      listDevices: () => ipcRenderer.invoke('native-audio-list-devices'),
      createDevice: (name, subDevices) => ipcRenderer.invoke('native-audio-create-device', { name, subDevices }),
      deleteDevice: (deviceId) => ipcRenderer.invoke('native-audio-delete-device', { deviceId }),
      requestPermission: () => ipcRenderer.invoke('native-audio-request-permission'),
      startCapture: (options) => ipcRenderer.invoke('native-audio-start-capture', options),
      stopCapture: () => ipcRenderer.invoke('native-audio-stop-capture'),
      isCapturing: () => ipcRenderer.invoke('native-audio-is-capturing')
    },
    
    // Dual Channel Recording API
    recording: {
      startDualChannel: (params) => ipcRenderer.invoke('start-audio-capture', params),
      stopDualChannel: () => ipcRenderer.invoke('stop-audio-capture'),
      compressAudio: (options) => ipcRenderer.invoke('compress-audio', options)
    },
    
    // Audio Streaming API
    streaming: {
      start: (params) => ipcRenderer.invoke('start-audio-streaming', params),
      stop: (params) => ipcRenderer.invoke('stop-audio-streaming', params),
      getStatus: () => ipcRenderer.invoke('get-streaming-status'),
      // Listen for streaming events
      onStatus: (callback) => {
        ipcRenderer.on('streaming-status', (event, data) => callback(data));
        return () => ipcRenderer.removeAllListeners('streaming-status');
      },
      onError: (callback) => {
        ipcRenderer.on('streaming-error', (event, data) => callback(data));
        return () => ipcRenderer.removeAllListeners('streaming-error');
      }
    },
    
    // Cue API (handles 2 audio websockets + insights websocket)
    cue: {
      start: (params) => ipcRenderer.invoke('start-cue', params),
      stop: () => ipcRenderer.invoke('stop-cue'),
      // Listen for Cue status updates
      onStatus: (callback) => {
        ipcRenderer.on('cue-status', (event, data) => callback(data));
        return () => ipcRenderer.removeAllListeners('cue-status');
      },
      // Listen for insights (TODO: will be implemented when insights websocket is added)
      onInsight: (callback) => {
        ipcRenderer.on('cue-insight', (event, data) => callback(data));
        return () => ipcRenderer.removeAllListeners('cue-insight');
      },
      // Listen for Cue errors
      onError: (callback) => {
        ipcRenderer.on('cue-error', (event, data) => callback(data));
        return () => ipcRenderer.removeAllListeners('cue-error');
      }
    },
    
    // File Upload API
    uploadFile: (options) => ipcRenderer.invoke('upload-file', { ...options }),
    uploadBothFiles: (options) => ipcRenderer.invoke('upload-both-files', { ...options }),

    // Permissions API
    permissions: {
      check: () => ipcRenderer.invoke('permissions-check'),
      requestAll: () => ipcRenderer.invoke('permissions-request-all')
    }
  });

  contextBridge.exposeInMainWorld('electronAPI', {
    resizeWindow: (width, height) => ipcRenderer.send('resize-coach-window', width, height),
    closeCoachWindow: () => ipcRenderer.send('close-coach-window')
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


