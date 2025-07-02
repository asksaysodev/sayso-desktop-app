export const useAudioCapture = () => {
  const startLiveCoach = async (params = {}) => {
    console.log('🎤 [useAudioCapture] Attempting to start live coach!');
    if (window.electron && window.electron.ipcRenderer) {
      try {
        // console.log('🎤 [useAudioCapture] Calling ipcRenderer.invoke("start-audio-capture")...');
        const result = await window.electron.ipcRenderer.invoke('start-audio-capture', params);
        // console.log('🎤 [useAudioCapture] ipcRenderer.invoke("start-audio-capture") successful, result:', result);
      } catch (error) {
        console.error('[useAudioCapture] Error invoking "start-audio-capture":', error);
      }
    } else {
      console.warn('[useAudioCapture] Electron ipcRenderer is not available. Cannot start live coach.');
    }
  };

  const stopLiveCoach = async () => {
    // console.log('[useAudioCapture] Attempting to stop live coach...');
    if (window.electron && window.electron.ipcRenderer) {
      try {
        // console.log('[useAudioCapture] Calling ipcRenderer.invoke("stop-audio-capture")...');
        const result = await window.electron.ipcRenderer.invoke('stop-audio-capture');
        // console.log('[useAudioCapture] ipcRenderer.invoke("stop-audio-capture") successful, result:', result);
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