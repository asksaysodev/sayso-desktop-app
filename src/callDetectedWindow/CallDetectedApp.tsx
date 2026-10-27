import React from 'react';

// Placeholder UI until the SAYSO-530 design lands. Main owns the 15s
// auto-hide; the progress bar here is purely visual.
const CallDetectedApp: React.FC = () => {
  const send = (channel: 'call-prompt:launch' | 'call-prompt:dismiss') => {
    window.electron?.ipcRenderer?.send(channel);
  };

  return (
    <div className="call-detected">
      <p className="call-detected-message">
        Looks like you've started a call. Do you want to launch Sayso?
      </p>
      <div className="call-detected-actions">
        <button className="call-detected-button" onClick={() => send('call-prompt:dismiss')}>
          Dismiss
        </button>
        <button
          className="call-detected-button call-detected-button--primary"
          onClick={() => send('call-prompt:launch')}
        >
          Launch Sayso
        </button>
      </div>
      <div className="call-detected-progress" />
    </div>
  );
};

export default CallDetectedApp;
