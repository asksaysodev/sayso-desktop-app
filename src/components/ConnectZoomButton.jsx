import React from 'react';
import '../styles/ZoomButton.css';
import { useAuth } from '../context/AuthContext';

const ConnectZoomButton = () => {
  const { globalUser } = useAuth();
    
  const handleConnect = () => {
    if(!globalUser) {
      console.log('No global user found');
      return;
    }

    window.location.href = `${import.meta.env.VITE_BACKEND_BASE_URL}/zoom/auth/${globalUser?.id}`;
  };

  return (
    <button onClick={handleConnect} className="zoom-button" disabled={!globalUser}>
      Connect Zoom Account
    </button>
  );
};

export default ConnectZoomButton;