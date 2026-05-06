import { useEffect, useState } from 'react';
import { FaHandPointer } from 'react-icons/fa6';

interface Props {
    x: number;
    y: number;
    clicking?: boolean;
}

export default function AnimatedCursor({ x, y, clicking = false }: Props) {
    const [rippleKey, setRippleKey] = useState(0);
    const [rippleVisible, setRippleVisible] = useState(false);

    useEffect(() => {
        if (!clicking) return;
        setRippleKey(k => k + 1);
        setRippleVisible(true);
        const t = setTimeout(() => setRippleVisible(false), 800);
        return () => clearTimeout(t);
    }, [clicking]);

    return (
        <div className="animated-cursor" style={{ left: x, top: y }}>
            {rippleVisible && (
                <div key={rippleKey} className="cursor-ripple">
                    <span className="ripple-ring ripple-ring--1" />
                    <span className="ripple-ring ripple-ring--2" />
                    <span className="ripple-ring ripple-ring--3" />
                </div>
            )}
            <FaHandPointer
                size={16}
                color="white"
                style={{ filter: 'drop-shadow(0 0 1.5px #0a0a1a) drop-shadow(0 0 1px #0a0a1a)' }}
            />
        </div>
    );
}
