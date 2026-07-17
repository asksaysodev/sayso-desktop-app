import { useState } from 'react';

interface Props {
    onStart: () => void;
    onSkip: () => void;
}

export default function IntroScreen({ onStart, onSkip }: Props) {
    const [exiting, setExiting] = useState(false);

    const handleStart = () => {
        setExiting(true);
        setTimeout(onStart, 350);
    };

    return (
        <div className={`onboarding-window intro-screen${exiting ? ' intro-screen--exiting' : ''}`}>
            <div className="onboarding-drag-bar">
                <span className="onboarding-drag-bar-text">Welcome to Sayso</span>
            </div>

            <div className="intro-content">
                <h1 className="intro-title intro-anim intro-anim--2">Welcome to Sayso</h1>
                <p className="intro-subtitle intro-anim intro-anim--3">
                    A quick guided tour — we'll point out where things live around your
                    screen. Just follow along and click when prompted.
                </p>
                <button className="intro-start-btn intro-pop" onClick={handleStart}>
                    Start tour
                </button>
            </div>

            <div className="intro-footer">
                <button className="onboarding-skip" onClick={onSkip}>
                    Skip for now
                </button>
            </div>
        </div>
    );
}
