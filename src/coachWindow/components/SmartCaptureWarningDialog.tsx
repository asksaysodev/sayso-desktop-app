import { forwardRef } from 'react';
import { LuX } from 'react-icons/lu';
import SaysoButton from '@/components/SaysoButton';
import '../styles/smartCaptureWarningDialog.css';

export type SmartCaptureWarningMode = 'reset' | 'stop';

interface Props {
    mode: SmartCaptureWarningMode;
    onPrimary: () => void;
    onSecondary: () => void;
    onDismiss: () => void;
    isProcessing?: boolean;
}

const COPY: Record<SmartCaptureWarningMode, { description: string; primary: string; secondary: string }> = {
    reset: {
        description: 'Smart Capture recorded notes in this session. Copy them before resetting?',
        primary: 'Copy & Reset',
        secondary: 'Skip & Reset',
    },
    stop: {
        description: 'Smart Capture recorded notes in this session. Copy them before ending?',
        primary: 'Copy & Stop',
        secondary: 'Skip & Stop',
    },
};

const SmartCaptureWarningDialog = forwardRef<HTMLDivElement, Props>(({ mode, onPrimary, onSecondary, onDismiss, isProcessing }, ref) => {
    const { description, primary, secondary } = COPY[mode];

    return (
        <div ref={ref} className="insights-vertical-layout-container smart-capture-warning-dialog">
            <button className="smart-capture-warning-dialog-close" onClick={onDismiss} aria-label="Dismiss" disabled={isProcessing}>
                <LuX size={14} />
            </button>
            <p className="smart-capture-warning-dialog-title">Save your notes first?</p>
            <p className="smart-capture-warning-dialog-description">{description}</p>
            <div className="smart-capture-warning-dialog-actions">
                <SaysoButton label={secondary} onClick={onSecondary} disabled={isProcessing} />
                <SaysoButton label={primary} onClick={onPrimary} loading={isProcessing} variant="outlined"/>
            </div>
        </div>
    );
});

SmartCaptureWarningDialog.displayName = 'SmartCaptureWarningDialog';

export default SmartCaptureWarningDialog;
