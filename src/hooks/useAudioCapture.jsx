export const useAudioCapture = () => { 
  const startLiveCoach = async (params = {}) => {
    console.log('🎤 [useAudioCapture] === ATTEMPTING TO START LIVE COACH ===');
    console.log('🎤 [useAudioCapture] Params:', JSON.stringify(params, null, 2));
    console.log('🎤 [useAudioCapture] Electron available:', !!window.electron);
    console.log('🎤 [useAudioCapture] IPC Renderer available:', !!(window.electron && window.electron.ipcRenderer));
    
    if (window.electron && window.electron.ipcRenderer) {
      try {
        console.log('🎤 [useAudioCapture] About to call ipcRenderer.invoke("start-audio-capture")...');
        const result = await window.electron.ipcRenderer.invoke('start-audio-capture', params);
        console.log('🎤 [useAudioCapture] ✅ ipcRenderer.invoke successful, result:', result);
      } catch (error) {
        console.error('🎤 [useAudioCapture] ❌ Error invoking "start-audio-capture":', error);
        console.error('🎤 [useAudioCapture] Error stack:', error.stack);
      }
    } else {
      console.error('🎤 [useAudioCapture] ❌ Electron ipcRenderer is not available!');
      console.error('🎤 [useAudioCapture] window.electron:', window.electron);
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