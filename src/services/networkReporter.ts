// Side-effect module: registers OS online/offline listeners and reports the
// current state to the main process. Every renderer entry point should import
// this once — especially the tray, which is the only window that exists during
// startup-offline and is therefore the only signal main has to recover.
//
// Main mirrors the reported state into global.networkState and uses the
// online event to lift AuthManager's offline pause. See docs/NETWORK_STATE_FLOW.md.

const reportNetworkStatus = (online: boolean) => {
  window.electron?.ipcRenderer?.send('network:report-status', online ? 'online' : 'offline');
};

window.addEventListener('online',  () => reportNetworkStatus(true));
window.addEventListener('offline', () => reportNetworkStatus(false));
reportNetworkStatus(navigator.onLine);
