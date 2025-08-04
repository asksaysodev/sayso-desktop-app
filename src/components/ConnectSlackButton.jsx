import React from 'react';
import '../styles/SlackButton.css';
import { useAuth } from '../context/AuthContext';
import { useSlack } from '../hooks/useSlack';
import {SiSlack} from 'react-icons/si'

// Check if we're in Electron environment
const isElectron = window.electron && window.electron.openExternal;

const ConnectSlackButton = () => {
  const { globalUser, updateGlobalUser } = useAuth();
  const { disconnectSlack, connectSlack } = useSlack(); 
  
  
  const handleConnect = async () => {
    
    if(!globalUser) {
      return;
    }

    if(globalUser?.slack_connected) {
      await disconnectSlack(globalUser?.id);
      updateGlobalUser(globalUser?.email); 
    } else {
      const authUrl = `${import.meta.env.VITE_BACKEND_BASE_URL}/slack/auth/${globalUser?.id}`;
      
      if (isElectron) {
        try {
          // In Electron: open in external browser and reset to home
          window.electron.openExternal(authUrl);
          // The main process will handle reset-to-home automatically
        } catch (error) {
          console.error('❌ [ConnectSlackButton] Error calling openExternal:', error);
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
    <button onClick={handleConnect} className={`slack-button ${globalUser?.slack_connected ? 'connected' : ''}`} disabled={!globalUser}>
      <SiSlack style={{marginRight: '10px'}} />
      <span>{
        globalUser?.slack_connected ? 'Disconnect Slack' : 'Connect Slack'
      }</span>
    </button>
  );
};

export default ConnectSlackButton;