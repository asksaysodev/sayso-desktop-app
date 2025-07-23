import React from 'react';
import '../styles/ZoomButton.css';
import { useAuth } from '../context/AuthContext';
import { useZoom } from '../hooks/useZoom';

// Check if we're in Electron environment
const isElectron = window.electron && window.electron.openExternal;

const ConnectZoomButton = () => {
  const { globalUser, updateGlobalUser } = useAuth();
  const { disconnectZoom } = useZoom();
  
  
  const handleConnect = async () => {
    
    if(!globalUser) {
      return;
    }

    if(globalUser?.zoom_connected) {
      await disconnectZoom(globalUser?.id);
      updateGlobalUser(globalUser?.email); 
    } else {
      const authUrl = `${import.meta.env.VITE_BACKEND_BASE_URL}/zoom/auth/${globalUser?.id}`;
      
      if (isElectron) {
        try {
          // In Electron: open in external browser and reset to home
          window.electron.openExternal(authUrl);
          // The main process will handle reset-to-home automatically
        } catch (error) {
          console.error('❌ [ConnectZoomButton] Error calling openExternal:', error);
          // Fallback to window.location.href
          window.location.href = authUrl;
        }
      } else {
        // In web browser: navigate normally
        window.location.href = authUrl;
      }
    }
    
  };

  return (
    <button onClick={handleConnect} className={`zoom-button ${globalUser?.zoom_connected ? 'connected' : ''}`} disabled={!globalUser}>
      {
        globalUser?.zoom_connected ? 'Disconnect Zoom Account' : 'Connect Zoom Account'
      }
    </button>
  );
};

export default ConnectZoomButton;