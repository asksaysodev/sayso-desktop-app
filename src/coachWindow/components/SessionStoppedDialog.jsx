import SaysoButton from '@/components/SaysoButton';
import { forwardRef } from 'react';
import '../styles/sessionStoppedDialog.css';

const SessionStoppedDialog = forwardRef(({ setShowSessionAutoStopped }, ref) => {
    
    return (
        <div ref={ref} className="insights-vertical-layout-container session-stopped-dialog">
            <p className="session-stopped-dialog-title">Session Stopped</p>
            <p className="session-stopped-dialog-description">We have stopped the session due to inactivity. You can launch a new session to continue.</p>
            <SaysoButton label="Understood" onClick={() => setShowSessionAutoStopped(false)} />
        </div>
    )
})

export default SessionStoppedDialog;