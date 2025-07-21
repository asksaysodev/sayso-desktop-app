import React from 'react';
import '../styles/ZoomButton.css';
import { useAuth } from '../context/AuthContext';
import { useZoom } from '../hooks/useZoom';

const ConnectZoomButton = () => {
  const { globalUser, updateGlobalUser } = useAuth();
  const { disconnectZoom } = useZoom();
  
  const handleConnect = async () => {
    if(!globalUser) {
      console.log('No global user found');
      return;
    }

    if(globalUser?.zoom_connected) {
      await disconnectZoom(globalUser?.id);
      updateGlobalUser(globalUser?.email); 
    } else {
      window.location.href = `${import.meta.env.VITE_BACKEND_BASE_URL}/zoom/auth/${globalUser?.id}`;
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