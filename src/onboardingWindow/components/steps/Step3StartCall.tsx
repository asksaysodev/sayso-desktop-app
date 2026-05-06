import { useEffect, useState } from 'react';
import AnimatedCursor from '../animations/AnimatedCursor';
import CoachWidgetMock from '../animations/CoachWidgetMock';

const delay = (ms: number) => new Promise(r => setTimeout(r, ms));

// Cursor positions relative to the step-scene (animation area ~720×403px)
// Widget is centered ~(360, 202). Drag area ~40px wide, then content starts ~240px from left.
// Lead type btn center x ≈ 320, Launch btn center x ≈ 430, both at y ≈ 202.
// Dropdown "Buyer Lead" first item ≈ y 245 (below button bottom + gap + item padding).
const LEAD_BTN = { x: 320, y: 180 };
const BUYER_LEAD_ITEM = { x: 335, y: 230 };
const LAUNCH_BTN = { x: 390, y: 180 };

export default function Step3StartCall() {
    const [cursorPos, setCursorPos] = useState({ x: 500, y: 300 });
    const [clicking, setClicking] = useState(false);
    const [leadType, setLeadType] = useState<string | null>(null);
    const [showDropdown, setShowDropdown] = useState(false);
    const [highlightDropdownItem, setHighlightDropdownItem] = useState<string | null>(null);
    const [animKey, setAnimKey] = useState(0);

    useEffect(() => {
        let active = true;
        const run = async () => {
            while (active) {
                // Reset
                setLeadType(null);
                setShowDropdown(false);
                setHighlightDropdownItem(null);
                setCursorPos({ x: 500, y: 300 });
                setAnimKey(k => k + 1);
                await delay(900);

                // Move to "Select lead type" button
                setCursorPos(LEAD_BTN);
                await delay(900);

                // Click → open dropdown
                setClicking(true);
                await delay(80);
                setClicking(false);
                setShowDropdown(true);
                await delay(500);

                // Move to "Buyer Lead" and highlight
                setCursorPos(BUYER_LEAD_ITEM);
                await delay(500);
                setHighlightDropdownItem('Buyer');
                await delay(300);

                // Click → select lead type
                setClicking(true);
                await delay(80);
                setClicking(false);
                setShowDropdown(false);
                setHighlightDropdownItem(null);
                setLeadType('Buyer');
                await delay(600);

                // Move to Launch button and click — session start shown in step 4
                setCursorPos(LAUNCH_BTN);
                await delay(800);

                setClicking(true);
                await delay(80);
                setClicking(false);
                await delay(1400);
            }
        };
        run();
        return () => { active = false; };
    }, []);

    return (
        <div className="step-scene step-scene--centered">
            <div key={animKey} className="step3-widget-wrap">
                <CoachWidgetMock
                    state="idle"
                    leadType={leadType}
                    showDropdown={showDropdown}
                    highlightDropdownItem={highlightDropdownItem}
                />
            </div>
            <AnimatedCursor x={cursorPos.x} y={cursorPos.y} clicking={clicking} />
        </div>
    );
}
