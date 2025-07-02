import '../styles/GlassedInsight.css';

export default function InsightPopUp({ message, isClosing }) { 
    console.log('message', message);
    console.log('isClosing', isClosing);

    return (
        <div className="glassed-insight-container-wrapper">
            <div className={`glassed-insight-container ${isClosing ? 'closing' : ''}`}>
                <h1>{message}</h1>
            </div>
        </div>
    )
}

