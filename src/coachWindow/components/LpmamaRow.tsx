import { useState, useRef, useLayoutEffect } from 'react';
import { useCoachWindowStore } from '../../store/coachWindowStore';
import { LpmamField } from '@/types/store/coachWindowStore';
import { Copy } from 'lucide-react';

const LPMAMA_CONFIG: { field: LpmamField; initial: string; label: string }[] = [
    { field: 'location',    initial: 'L', label: 'Location' },
    { field: 'price',       initial: 'P', label: 'Price' },
    { field: 'motivation',  initial: 'M', label: 'Motivation' },
    { field: 'agent',       initial: 'A', label: 'Agent' },
    { field: 'mortgage',    initial: 'M', label: 'Mortgage' },
    { field: 'appointment', initial: 'A', label: 'Appointment' },
];

// Distance from row bottom to tooltip top — matches the CSS `top: calc(100% + 9px)` on .lpmama-tooltip
const TOOLTIP_OFFSET = 9;

interface Props {
    onTooltipHeightChange: (height: number) => void;
}

export default function LpmamaRow({ onTooltipHeightChange }: Props) {
    const lpmama = useCoachWindowStore(state => state.cue.lpmama);
    const [hoveredField, setHoveredField] = useState<LpmamField | null>(null);
    const tooltipRef = useRef<HTMLDivElement | null>(null);

    useLayoutEffect(() => {
        if (!hoveredField || !tooltipRef.current) {
            onTooltipHeightChange(0);
            return;
        }
        onTooltipHeightChange(tooltipRef.current.offsetHeight + TOOLTIP_OFFSET);
    }, [hoveredField, onTooltipHeightChange]);

    const copyLpmamaContent = async () => {
        const textToCopy = LPMAMA_CONFIG.map(({ field, label }) => `${label}: ${lpmama[field] ?? ''}`).join('\n');
        try {
            await navigator.clipboard.writeText(textToCopy);
        } catch {
            // clipboard write failed — silently ignore
        }
    }
    
    return (
        <div className="lpmama-row">
            {LPMAMA_CONFIG.map(({ field, initial, label }) => {
                const content = lpmama[field];
                const hasData = content !== null;
                const isHovered = hoveredField === field;
                const tooltipText = hasData ? `${label}: ${content}` : `${label}: No data yet`;

                return (
                    <div key={field} className="lpmama-dot-wrapper" data-field={field}>
                        <div
                            className={`lpmama-dot${hasData ? ' lpmama-dot--captured' : ''}`}
                            onMouseEnter={() => setHoveredField(field)}
                            onMouseLeave={() => setHoveredField(null)}
                        >
                            {initial}
                        </div>
                        {isHovered && (
                            <div ref={tooltipRef} className="lpmama-tooltip">
                                <div className="lpmama-tooltip-arrow" />
                                <span className="lpmama-tooltip-text">{tooltipText}</span>
                            </div>
                        )}
                    </div>
                );
            })}
            <div className='lpmama-dot' onClick={copyLpmamaContent}>
                <Copy size={14} />
            </div>
        </div>
    );
}
