const SAYSO_PATH = "M38.11,47.13c-.09.28-.27.54-.48.71.08-.08.12-.18.17-.28.08-.17.17-.31.31-.43ZM58.78,42.42c0,3.24-1.73,6.06-4.34,7.62l-.24.15-.09.05L6.09,80.67l-.07.04s-.02,0-.03,0c-.17.11-.37.17-.59.17-.65,0-1.17-.52-1.17-1.17,0-.28.1-.54.27-.75,0-.03,0-.06.02-.07l33-30.94s.07-.07.09-.1,0-.01,0-.02c.22-.17.39-.43.48-.71h0c.05-.16.08-.32.08-.49,0-.9-.72-1.62-1.61-1.62H8.89c-4.9,0-8.89-3.99-8.89-8.89,0-3.38,1.88-6.32,4.66-7.83l.04-.02L49.23.2l.04-.03s.02,0,.03,0h0c.17-.1.37-.16.59-.16.64,0,1.17.52,1.17,1.17,0,.28-.1.55-.27.75,0,.03,0,.05-.02.06l-29.51,28.61s-.04.05-.05.07c-.25.19-.44.45-.53.77-.06.15-.09.32-.09.49,0,.89.72,1.61,1.61,1.61h27.69c4.91,0,8.89,3.99,8.89,8.9Z";

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
                {/* <div className="intro-brand intro-anim intro-anim--1">
                    <span className="intro-brand-logo">
                        <svg viewBox="0 0 58.78 80.89" width="15" height="15">
                            <path d={SAYSO_PATH} fill="white" />
                        </svg>
                    </span>
                    <span className="intro-brand-name">Sayso</span>
                </div> */}

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
