import { useEffect, useState } from 'react';
import AnimatedCursor from '../animations/AnimatedCursor';
import MacBookMock from '../animations/MacBookMock';
import MenuBarMock from '../animations/MenuBarMock';

const delay = (ms: number) => new Promise(r => setTimeout(r, ms));

const SAYSO_X = 370;
const SAYSO_Y = 80;

export default function Step1TrayIcon() {
    const [cursorPos, setCursorPos] = useState({ x: 490, y: 300 });
    const [clicking, setClicking] = useState(false);
    const [saysoActive, setSaysoActive] = useState(false);

    useEffect(() => {
        let active = true;
        const run = async () => {
            while (active) {
                setCursorPos({ x: 490, y: 200 });
                setSaysoActive(false);
                await delay(900);

                setCursorPos({ x: SAYSO_X, y: SAYSO_Y });
                await delay(1100);

                setSaysoActive(true);
                setClicking(true);
                await delay(80);
                setClicking(false);
                await delay(2000);

                setSaysoActive(false);
                await delay(0);
            }
        };
        run();
        return () => { active = false; };
    }, []);

    return (
        <div className="step-scene step-scene--centered">
            <MacBookMock>
                <MenuBarMock saysoActive={saysoActive} step1 />
            </MacBookMock>
            <AnimatedCursor x={cursorPos.x} y={cursorPos.y} clicking={clicking} />
        </div>
    );
}
