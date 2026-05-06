import { useEffect, useState } from 'react';
import AnimatedCursor from '../animations/AnimatedCursor';
import CoachWidgetMock from '../animations/CoachWidgetMock';

const delay = (ms: number) => new Promise(r => setTimeout(r, ms));

function formatTime(s: number) {
    const h = Math.floor(s / 3600);
    const m = Math.floor((s % 3600) / 60);
    const sec = s % 60;
    return `${h}:${m.toString().padStart(2, '0')}:${sec.toString().padStart(2, '0')}`;
}

const STOP_BTN = { x: 394, y: 180 };

export default function Step4StopSession() {
    const [cursorPos, setCursorPos] = useState({ x: 500, y: 300 });
    const [clicking, setClicking] = useState(false);
    const [elapsed, setElapsed] = useState(0);

    useEffect(() => {
        const id = setInterval(() => setElapsed(s => s + 1), 1000);
        return () => clearInterval(id);
    }, []);

    useEffect(() => {
        let active = true;
        const run = async () => {
            while (active) {
                setCursorPos({ x: 500, y: 300 });
                await delay(900);

                setCursorPos(STOP_BTN);
                await delay(900);

                setClicking(true);
                await delay(80);
                setClicking(false);
                await delay(1600);
            }
        };
        run();
        return () => { active = false; };
    }, []);

    return (
        <div className="step-scene step-scene--centered">
            <CoachWidgetMock
                state="active"
                leadType="Buyer"
                timer={formatTime(elapsed)}
            />
            <AnimatedCursor x={cursorPos.x} y={cursorPos.y} clicking={clicking} />
        </div>
    );
}
