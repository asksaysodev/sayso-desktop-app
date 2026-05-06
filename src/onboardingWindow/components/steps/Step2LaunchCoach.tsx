import { useEffect, useState } from 'react';
import AnimatedCursor from '../animations/AnimatedCursor';
import TrayMenuMock from '../animations/TrayMenuMock';

const delay = (ms: number) => new Promise(r => setTimeout(r, ms));

export default function Step2LaunchCoach() {
    const [cursorPos, setCursorPos] = useState({ x: 525, y: 10 });
    const [clicking, setClicking] = useState(false);
    const [highlighted, setHighlighted] = useState(false);
    const [showMenu, setShowMenu] = useState(true);

    useEffect(() => {
        let active = true;
        const run = async () => {
            while (active) {
                setShowMenu(true);
                setHighlighted(false);
                setCursorPos({ x: 525, y: 10 });
                await delay(700);

                setCursorPos({ x: 560, y: 102 });
                await delay(900);
                setHighlighted(true);
                await delay(400);

                setClicking(true);
                await delay(80);
                setClicking(false);
                await delay(300);

                await delay(2000);
            }
        };
        run();
        return () => { active = false; };
    }, []);

    return (
        <div className="step-scene">
            {showMenu && (
                <div className="step-tray-menu-anchor">
                    <TrayMenuMock highlightLaunchCoach={highlighted} />
                </div>
            )}
            <AnimatedCursor x={cursorPos.x} y={cursorPos.y} clicking={clicking} />
        </div>
    );
}
