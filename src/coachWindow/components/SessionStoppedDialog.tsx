import SaysoButton from '@/components/SaysoButton';
import { forwardRef } from 'react';
import '../styles/sessionStoppedDialog.css';

export type SessionStoppedReason = 'inactivity' | 'expired';

const DESCRIPTIONS: Record<SessionStoppedReason, string> = {
    inactivity: 'We have stopped the session due to inactivity. You can launch a new session to continue.',
    expired: 'This session is no longer available, so we have stopped it. You can launch a new session to continue.'
};

interface SessionStoppedDialogProps {
    setShowSessionAutoStopped: (show: boolean) => void;
    reason?: SessionStoppedReason;
}

const SessionStoppedDialog = forwardRef<HTMLDivElement, SessionStoppedDialogProps>(({ setShowSessionAutoStopped, reason = 'inactivity' }, ref) => {

    return (
        <div ref={ref} className="insights-vertical-layout-container session-stopped-dialog">
            <p className="session-stopped-dialog-title">Session Stopped</p>
            <p className="session-stopped-dialog-description">{DESCRIPTIONS[reason]}</p>
            <SaysoButton label="Understood" onClick={() => setShowSessionAutoStopped(false)} />
        </div>
    )
})

export default SessionStoppedDialog;