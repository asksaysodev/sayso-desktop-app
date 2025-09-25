import SmartCaptureItem from './SmartCaptureItem';

import '../styles/SmartCaptureBox.css';

export default function SmartCaptureBox({signals}) {

    const getLatestSignal = (signalType) => {
        if (!signals || !Array.isArray(signals)) return null;
        
        const filteredSignals = signals.filter(s => s.signal === signalType);
        if (filteredSignals.length === 0) return null;
        
        const latestSignal = filteredSignals[filteredSignals.length - 1];
        return {
            detected: latestSignal.detected,
            quote: latestSignal.quote
        };
    };

    return (
        <div className='smart-capture-container'>
            <ul>
                <SmartCaptureItem signal={getLatestSignal('pain_point')} title="Pain Points"/>
                <div className='smart-capture-item-divider'></div>
                <SmartCaptureItem signal={getLatestSignal('economic_impact')} title="Economic Impact" />
                <div className='smart-capture-item-divider'></div>
                <SmartCaptureItem signal={getLatestSignal('decision_maker')} title="Decision Makers" />
                <div className='smart-capture-item-divider'></div>
                <SmartCaptureItem signal={getLatestSignal('objection')} title="Buying Process Objections" />
            </ul>
        </div>
    )
}