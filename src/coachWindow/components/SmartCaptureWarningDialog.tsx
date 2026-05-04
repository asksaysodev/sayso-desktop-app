import { forwardRef } from 'react';
import SaysoButton from '@/components/SaysoButton';
import '../styles/smartCaptureWarningDialog.css';

export type SmartCaptureWarningMode = 'reset' | 'stop';

interface Props {
    mode: SmartCaptureWarningMode;
    onPrimary: () => void;
    onSecondary: () => void;
}

const COPY: Record<SmartCaptureWarningMode, { description: string; primary: string; secondary: string }> = {
    reset: {
        description: 'Smart Capture recorded notes in this session. Copy them before resetting?',
        primary: 'Copy & Reset',
        secondary: 'Reset Anyway',
    },
    stop: {
        description: 'Smart Capture recorded notes in this session. Copy them before ending?',
        primary: 'Copy & Stop',
        secondary: 'Stop Anyway',
    },
};

const SmartCaptureWarningDialog = forwardRef<HTMLDivElement, Props>(({ mode, onPrimary, onSecondary }, ref) => {
    const { description, primary, secondary } = COPY[mode];

    return (
        <div ref={ref} className="insights-vertical-layout-container smart-capture-warning-dialog">
            <p className="smart-capture-warning-dialog-title">Unsaved capture data</p>
            <p className="smart-capture-warning-dialog-description">{description}</p>
            <div className="smart-capture-warning-dialog-actions">
                <SaysoButton label={secondary} onClick={onSecondary} variant="outlined" />
                <SaysoButton label={primary} onClick={onPrimary} />
            </div>
        </div>
    );
});

SmartCaptureWarningDialog.displayName = 'SmartCaptureWarningDialog';

export default SmartCaptureWarningDialog;
