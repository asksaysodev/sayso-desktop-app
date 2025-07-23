import '../styles/GlassedInsight.css';

export default function InsightPopUp({ message, isClosing, isIceBreaker }) { 

    return (
        <div className="glassed-insight-container-wrapper">
            <div className={`glassed-insight-container ${isClosing ? 'closing' : ''}`}>
                <h1>{isIceBreaker && '🧊 Ice Breaker: '}{message}</h1>
            </div>
        </div>
    )
}

