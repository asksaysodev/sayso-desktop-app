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

export default function InsightWrapper({ onComplete, insightText, displayDuration, transitionDelay, animationDuration }) {
    const [isVisible, setIsVisible] = useState(false);
    const [isExiting, setIsExiting] = useState(false);

    useEffect(() => {
        const calculatedDisplayDuration = calculateInsightDisplayDuration(insightText);
        const finalDisplayDuration = calculatedDisplayDuration || displayDuration;

        const enterTimer = setTimeout(() => {
            setIsVisible(true);
        }, 10);

        const exitTimer = setTimeout(() => {
            setIsExiting(true);
        }, finalDisplayDuration - animationDuration);

        const completeTimer = setTimeout(() => {
            onComplete();
        }, finalDisplayDuration + transitionDelay);

        return () => {
            clearTimeout(enterTimer);
            clearTimeout(exitTimer);
            clearTimeout(completeTimer);
        };
    }, [insightText, displayDuration, transitionDelay, animationDuration, onComplete]);


    return (
        <div
        className={`toast ${isVisible ? 'toast--visible' : ''} ${isExiting ? 'toast--exiting' : ''}`}
        style={{
            '--animation-duration': `${animationDuration}ms`,
        }}
    >
        <div className="toast__text">{insightText}</div>
    </div>
    )
}