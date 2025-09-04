const { contextBridge, ipcRenderer, app } = require('electron');

// It's a good practice to log if the preload script is loaded
// console.log('Preload script initializing...');

try {
  contextBridge.exposeInMainWorld('electron', {
    ipcRenderer: {
      invoke: (channel, ...args) => {
        // console.log(`Preload: ipcRenderer.invoke('${channel}', args:`, args, ')');
        return ipcRenderer.invoke(channel, ...args);
      },
      on: (channel, callback) => {
        console.log('🔌 PRELOAD: Setting up listener for channel:', channel);
        ipcRenderer.on(channel, (event, ...args) => {
          console.log('📨 PRELOAD: Received data on channel:', channel, args);
          // Pass the first argument directly since we're sending a single object
          callback(args[0]);
        });
        return () => {
          console.log('🧹 PRELOAD: Cleaning up listener for channel:', channel);
          ipcRenderer.removeAllListeners(channel);
        };
      },
      send: (channel, ...args) => {
        // console.log(`Preload: ipcRenderer.send('${channel}', args:`, args, ')');
        ipcRenderer.send(channel, ...args);
      },
      removeAllListeners: (channel) => {
        // console.log(`Preload: ipcRenderer.removeAllListeners('${channel}')`);
        ipcRenderer.removeAllListeners(channel);
      }
      // You can add other specific methods you frequently use if desired
    },
    // Add openExternal method for opening URLs in external browser
    openExternal: (url) => {
      ipcRenderer.send('open-external', url);
    }
  });

  contextBridge.exposeInMainWorld('electronAPI', {
    resizeWindow: (height) => ipcRenderer.send('resize-coach-window', height)
  });

  // Extract --indexHtmlPath from process.argv
  const indexHtmlPathArg = process.argv.find(arg => arg.startsWith('--indexHtmlPath='));
  const indexHtmlPath = indexHtmlPathArg ? indexHtmlPathArg.replace('--indexHtmlPath=', '') : '';

  contextBridge.exposeInMainWorld('sayso', {
    indexHtmlPath
  });

  // console.log('Preload: contextBridge.exposeInMainWorld for "electron" successful.');

} catch (error) {
  console.error('Preload Error: Failed to exposeInMainWorld:', error);
}

// Make sure contextIsolation is true and nodeIntegration is false
// in your BrowserWindow settings in main.cjs for this to be secure and work correctly.

// You can also listen for messages from main here if needed,
// but for exposing APIs, the above is the primary pattern.
ipcRenderer.on('main-process-message', (_event, message) => {
  // console.log('Message from main process:', message);
});

// Notify that preload script has finished loading (optional)
// window.addEventListener('DOMContentLoaded', () => {
//   console.log('Preload: DOMContentLoaded, renderer is ready.');
//   ipcRenderer.send('renderer-ready'); // Example: notify main
// });

// Clean up listeners when the window is unloaded (good practice)
// window.addEventListener('beforeunload', () => {
  // console.log('Preload: beforeunload, cleaning up listeners for "electron.ipcRenderer"');
  // if (window.electron && window.electron.ipcRenderer && typeof window.electron.ipcRenderer.removeAllListeners === 'function') {
    // This won't work as expected because removeAllListeners is on the original ipcRenderer, not the exposed one.
    // Cleanup needs to be managed via the returned functions from .on() or by exposing removeAllListeners correctly.
  // }
// }); 