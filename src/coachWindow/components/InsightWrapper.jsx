import { useState, useEffect } from 'react';
import calculateInsightDisplayDuration from "../helpers/calculateInsightDisplayDuration";
import '../styles/Cue.css';

{/**
    interface InsightWrapperProps {
        // Function to be called when the insight is complete (next())
        onComplete: () => void;

        priority: "high" | "mid" | "low";
        insightText: string;

        // Display duration of the toast
        displayDuration: number;
        //Time between toasts - allows exit animation (300ms) to complete with buffer
        transitionDelay: number;
        // Animation duration of the toast
        animationDuration: number;
    }
 */}

export default function InsightWrapper({ onComplete, priority, insightText, displayDuration, transitionDelay, animationDuration }) {
    const [isVisible, setIsVisible] = useState(false);
    const [isExiting, setIsExiting] = useState(false);

    useEffect(() => {
        console.log('InsightText', insightText);
        const calculatedDisplayDuration = calculateInsightDisplayDuration(insightText);
        const finalDisplayDuration = calculatedDisplayDuration || displayDuration;
        console.log('finalDisplayDuration', finalDisplayDuration);

        const enterTimer = setTimeout(() => {
            console.log('Setting isVisible to true');
            setIsVisible(true);
        }, 10);

        const exitTimer = setTimeout(() => {
            console.log('Setting isExiting to true');
            setIsExiting(true);
        }, finalDisplayDuration - animationDuration);

        const completeTimer = setTimeout(() => {
            console.log('Calling onComplete');
            onComplete();
        }, finalDisplayDuration + transitionDelay);

        return () => {
            console.log('Cleaning up timers');
            clearTimeout(enterTimer);
            clearTimeout(exitTimer);
            clearTimeout(completeTimer);
        };
    }, [insightText, displayDuration, transitionDelay, animationDuration, onComplete]);


    return (
        <div
        className={`toast toast--${priority} ${isVisible ? 'toast--visible' : ''} ${isExiting ? 'toast--exiting' : ''}`}
        style={{
            '--animation-duration': `${animationDuration}ms`,
        }}
    >
        {/* <div className="toast__content">
            <div className="toast__priority-badge">{priority}</div>
            <div className="toast__text">{insightText}</div>
        </div> */}
        <div className="toast__text">{insightText}</div>
    </div>
    )
}