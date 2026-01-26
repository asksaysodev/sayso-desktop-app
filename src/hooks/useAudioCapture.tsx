export const useAudioCapture = () => { 
  const startLiveCoach = async (params = {}) => {
    
    if (window.electron && window.electron.ipcRenderer) {
      try {
        const result = await window.electron.ipcRenderer.invoke('start-audio-capture', params);
      } catch (error) {
        console.error('🎤 [useAudioCapture] ❌ Error invoking "start-audio-capture":', error);
      }
    } else {
      console.error('🎤 [useAudioCapture] ❌ Electron ipcRenderer is not available!');
      console.error('🎤 [useAudioCapture] window.electron:', window.electron);
    }
  };

  const stopLiveCoach = async () => {
    if (window.electron && window.electron.ipcRenderer) {
      try {
        await window.electron.ipcRenderer.invoke('stop-audio-capture');
      } catch (error) {
        console.error('[useAudioCapture] Error invoking "stop-audio-capture":', error);
      }
    } else {
      console.warn('[useAudioCapture] Electron ipcRenderer is not available. Cannot stop live coach.');
    }
  };

  return {
    startLiveCoach,
    stopLiveCoach,
  }
}