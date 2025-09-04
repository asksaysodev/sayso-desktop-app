export default function useAudio() { 
    const startLiveCoach = async (params = {}) => {
      
      if (window.electron && window.electron.ipcRenderer) {
        try {
          await window.electron.ipcRenderer.invoke('start-audio-capture', params);
        } catch (error) {
          console.error('🎤 [useAudio] ❌ Error invoking "start-audio-capture":', error);
        }
      } else {
        console.error('🎤 [useAudio] ❌ Electron ipcRenderer is not available!');
        console.error('🎤 [useAudio] window.electron:', window.electron);
      }
    };
  
    const stopLiveCoach = async () => {
      if (window.electron && window.electron.ipcRenderer) {
        try {
          await window.electron.ipcRenderer.invoke('stop-audio-capture');
        } catch (error) {
          console.error('[useAudio] Error invoking "stop-audio-capture":', error);
        }
      } else {
        console.warn('[useAudio] Electron ipcRenderer is not available. Cannot stop live coach.');
      }
    };
  
    return {
      startLiveCoach,
      stopLiveCoach,
    }
  }