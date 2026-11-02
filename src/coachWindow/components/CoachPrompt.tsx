import React, { useEffect, useRef, useState } from 'react';
import { X } from 'lucide-react';
import { LeadTypeEnum } from '@/types/coach';
import '../styles/CoachPrompt.css';

// SAYSO-566: the coach window's prompt state, shown when call detection spots a
// call (electron/callDetection/callPrompt.ts owns when). Hovering Start splits
// it into "Start for Seller" / "Start for Buyer" like two drops pulling apart:
// two blobs under an SVG goo filter, labels on a layer above it so the filter
// never blurs text. Main owns the auto-hide; the ring around the close button
// runs for the same duration and pauses with it on hover. On timeout or dismiss
// main sends call-prompt:exit and closes the window once the exit has played.
// Picking a lead fades both pills out and "Starting conversation" in (copy, not
// a button). The prompt keeps its look and width until the session is up; then
// the coach view takes over and grows straight to the active width
// (CoachWindowApp.tsx).

const SaysoMark: React.FC = () => (
    <svg viewBox="0 0 58.78 80.89" fill="currentColor" aria-hidden="true">
        <path d="M58.78,42.42c0,3.24-1.73,6.06-4.34,7.62l-.24.15-.09.05L6.09,80.67l-.07.04s-.02,0-.03,0c-.17.11-.37.17-.59.17-.65,0-1.17-.52-1.17-1.17,0-.28.1-.54.27-.75,0-.03,0-.06.02-.07l33-30.94s.07-.07.09-.1t0-.02c.22-.17.39-.43.48-.71h0c.05-.16.08-.32.08-.49,0-.9-.72-1.62-1.61-1.62H8.89c-4.9,0-8.89-3.99-8.89-8.89,0-3.38,1.88-6.32,4.66-7.83l.04-.02L49.23.2l.04-.03s.02,0,.03,0h0c.17-.1.37-.16.59-.16.64,0,1.17.52,1.17,1.17,0,.28-.1.55-.27.75,0,.03,0,.05-.02.06l-29.51,28.61s-.04.05-.05.07c-.25.19-.44.45-.53.77-.06.15-.09.32-.09.49,0,.89.72,1.61,1.61,1.61h27.69c4.91,0,8.89,3.99,8.89,8.9Z" />
    </svg>
);

interface Props {
    /** A lead pill was picked and main turned this window into the coach: start
        the session (the app swaps to the coach view once it's up). */
    onLaunched: (leadType: LeadTypeEnum) => void;
    /** The coach was opened from elsewhere while the prompt was up: swap to the coach view. */
    onPromoted: () => void;
}

export default function CoachPrompt({ onLaunched, onPromoted }: Props) {
    const [exiting, setExiting] = useState(false);
    // Mirrors main's auto-hide pause, so the countdown stops with the timer
    const [hovered, setHovered] = useState(false);
    const [split, setSplit] = useState(false);
    const [starting, setStarting] = useState<LeadTypeEnum | null>(null);
    // A launch's own promotion is handled by the app, once the session is up
    const startingRef = useRef(false);

    useEffect(() => {
        return window.electron?.ipcRenderer?.on('call-prompt:exit', () => setExiting(true));
    }, []);

    useEffect(() => {
        return window.electron?.ipcRenderer?.on('call-prompt:promoted', () => {
            if (!startingRef.current) onPromoted();
        });
    }, [onPromoted]);

    const handleHover = (value: boolean) => {
        setHovered(value);
        window.electron?.ipcRenderer?.send('call-prompt:hover', value);
    };

    // Main runs the mic check and, if it passes, turns this window into the
    // coach before answering; false means it closed the prompt instead. The
    // session starts as soon as main says yes, and this view stays (showing
    // "Starting conversation") until the app swaps to the coach.
    const handleLaunch = async (leadType: LeadTypeEnum) => {
        if (startingRef.current) return;
        startingRef.current = true;
        setStarting(leadType);
        const launched = await window.electron?.ipcRenderer?.invoke('call-prompt:launch');
        if (launched === true) {
            onLaunched(leadType);
            return;
        }
        startingRef.current = false;
        setStarting(null);
    };

    const classes = [
        'coach-prompt',
        exiting && 'coach-prompt--exiting',
        hovered && 'coach-prompt--paused',
        (split || starting) && 'coach-prompt--split',
        starting && 'coach-prompt--starting',
    ].filter(Boolean).join(' ');

    return (
        <div className={classes}>
            <svg className="coach-prompt-defs" aria-hidden="true">
                <filter id="coach-prompt-goo">
                    <feGaussianBlur in="SourceGraphic" stdDeviation="5" result="blur" />
                    <feColorMatrix in="blur" values="1 0 0 0 0  0 1 0 0 0  0 0 1 0 0  0 0 0 18 -7" />
                </filter>
            </svg>

            <div
                className="coach-prompt-pill"
                onMouseEnter={() => handleHover(true)}
                onMouseLeave={() => handleHover(false)}
            >
                <div className="coach-prompt-mark">
                    <SaysoMark />
                </div>

                <div
                    className="coach-prompt-cta"
                    onMouseLeave={() => { if (!starting) setSplit(false); }}
                    onFocus={() => setSplit(true)}
                    onBlur={(e) => {
                        if (!e.currentTarget.contains(e.relatedTarget as Node)) setSplit(false);
                    }}
                >
                    <span className="coach-prompt-copy">Sayso identified a call</span>

                    <div className="coach-prompt-goo" aria-hidden="true">
                        <span className="coach-prompt-blob coach-prompt-blob--buyer" />
                        <span className="coach-prompt-blob coach-prompt-blob--seller" />
                    </div>

                    <span className="coach-prompt-start" aria-hidden={split}>
                        Start
                    </span>
                    <button
                        className="coach-prompt-lead coach-prompt-lead--seller"
                        tabIndex={split ? 0 : -1}
                        disabled={starting !== null}
                        onClick={() => handleLaunch(LeadTypeEnum.SELLER)}
                    >
                        Start for Seller
                    </button>
                    <button
                        className="coach-prompt-lead coach-prompt-lead--buyer"
                        tabIndex={split ? 0 : -1}
                        disabled={starting !== null}
                        onClick={() => handleLaunch(LeadTypeEnum.BUYER)}
                    >
                        Start for Buyer
                    </button>
                    <span className="coach-prompt-starting" aria-hidden={!starting}>
                        Starting conversation
                    </span>
                    {/* Hover or keyboard target while collapsed: only Start splits the CTA,
                        not the copy beside it. Leaving the whole area collapses it again. */}
                    {!split && (
                        <button
                            className="coach-prompt-start-target"
                            aria-label="Start"
                            onMouseEnter={() => setSplit(true)}
                        />
                    )}
                </div>

                <button
                    className="coach-prompt-close"
                    // Starting: the prompt is already the coach, nothing left to dismiss
                    disabled={starting !== null}
                    onClick={() => window.electron?.ipcRenderer?.send('call-prompt:dismiss')}
                    aria-label="Dismiss"
                >
                    <svg className="coach-prompt-ring" viewBox="0 0 36 36" aria-hidden="true">
                        <circle className="coach-prompt-ring-track" cx="18" cy="18" r="17" />
                        <circle className="coach-prompt-ring-progress" cx="18" cy="18" r="17" pathLength="100" />
                    </svg>
                    <X size={16} />
                </button>
            </div>
        </div>
    );
}
