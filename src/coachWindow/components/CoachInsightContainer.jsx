import { useState, useEffect } from 'react';

export default function CoachInsightContainer({displayInsight}) {
    const [isVisible, setIsVisible] = useState(false);
    const [isClosing, setIsClosing] = useState(false);

    useEffect(() => {
        if (displayInsight) {
            setIsVisible(true);
            setIsClosing(false);
        } else if (isVisible) {
            // Start closing animation
            setIsClosing(true);
            // Remove from DOM after animation completes
            const timer = setTimeout(() => {
                setIsVisible(false);
                setIsClosing(false);
            }, 200); // Match the CSS animation duration
            
            return () => clearTimeout(timer);
        }
    }, [displayInsight, isVisible]);

    if (!isVisible) return null;

    return (
        <div className={`coach-window-insights-container coach-box-bubble ${isClosing ? 'closing' : ''}`}>
            <p>{displayInsight}</p>
        </div>
    );
}