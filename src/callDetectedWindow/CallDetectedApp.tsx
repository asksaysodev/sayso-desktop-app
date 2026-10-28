import React, { useEffect, useState } from 'react';
import saysoMark from '/assets/sayso.svg';

// SAYSO-530 design. Main owns the auto-hide; the countdown in Dismiss is
// purely visual and runs for the same duration, pausing with it on hover. On timeout or dismiss main
// sends call-prompt:exit and closes the window once the slide-out has played.
const CallDetectedApp: React.FC = () => {
  const [exiting, setExiting] = useState(false);
  // Mirrors main's auto-hide pause, so the countdown stops with the timer
  const [hovered, setHovered] = useState(false);

  useEffect(() => {
    return window.electron?.ipcRenderer?.on('call-prompt:exit', () => setExiting(true));
  }, []);

  const send = (channel: 'call-prompt:launch' | 'call-prompt:dismiss') => {
    window.electron?.ipcRenderer?.send(channel);
  };

  const handleHover = (value: boolean) => {
    setHovered(value);
    window.electron?.ipcRenderer?.send('call-prompt:hover', value);
  };

  return (
    <div
      className={`call-detected${exiting ? ' call-detected--exiting' : ''}${hovered ? ' call-detected--paused' : ''}`}
      onMouseEnter={() => handleHover(true)}
      onMouseLeave={() => handleHover(false)}
    >
      <div className="call-detected-header">
        <div className="call-detected-logo">
          <img src={saysoMark} alt="Sayso" />
        </div>
        <p className="call-detected-message">
          Looks like you've started a call. Do you want to launch Sayso?
        </p>
      </div>
      <div className="call-detected-actions">
        <button className="call-detected-button" onClick={() => send('call-prompt:dismiss')}>
          <span className="call-detected-countdown" aria-hidden="true" />
          <span className="call-detected-button-label">Dismiss</span>
        </button>
        <button
          className="call-detected-button call-detected-button--primary"
          onClick={() => send('call-prompt:launch')}
        >
          <span className="call-detected-button-label">Launch Coach</span>
        </button>
      </div>
    </div>
  );
};

export default CallDetectedApp;
