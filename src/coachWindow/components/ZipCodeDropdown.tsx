import { forwardRef, useState, useEffect, useRef } from 'react';
import { Minus, MoveUpRight, MoveDownRight, Equal, Info } from 'lucide-react';
import { Spinner } from '@/components/ui/spinner';
import { MdErrorOutline } from 'react-icons/md';
import { LuRefreshCw } from 'react-icons/lu';
import { PulseResponse, PulseApiError, MarketDataItem } from '../services/getPulseMarketProperty';


const TREND_CONFIG = {
    up:      { Icon: MoveUpRight,   className: 'zc-trend-up' },
    down:    { Icon: MoveDownRight,  className: 'zc-trend-down' },
    neutral: { Icon: Equal,          className: 'zc-trend-neutral' },
} as const;

function renderValue(item: MarketDataItem) {
    const { value } = item;
    if (value === null || value === undefined) {
        return <span className='zc-fact-value'>—</span>;
    }
    if (typeof value === 'object' && 'direction' in value) {
        const { Icon, className } = TREND_CONFIG[value.direction];
        return (
            <span className='zc-fact-value zc-fact-value--trend'>
                <span className={`zc-trend ${className}`}>
                    <Icon size={14} className='zc-trend-icon' />
                </span>
            </span>
        );
    }
    return <span className='zc-fact-value'>{String(value)}</span>;
}

function getErrorDisplay(error: PulseApiError, zip: string): { text: string; canRetry: boolean } {
    if (error.status === 404) return { text: `No market data available for ZIP ${zip}.`, canRetry: false };
    if (error.status === 502) return { text: 'Pulse data is temporarily unavailable.', canRetry: true };
    return { text: 'Something went wrong.', canRetry: true };
}

interface Props {
    onClose: () => void;
    zipCodeValue: string;
    allResults: PulseResponse | null;
    pulseError: PulseApiError | null;
    isPending: boolean;
    onRetry: () => void;
}

interface TooltipState {
    key: string;
    detail: string;
    bottomPx: number;
    side: 'left' | 'right';
}

const ZipCodeDropdown = forwardRef<HTMLDivElement, Props>(({
    onClose,
    zipCodeValue,
    allResults,
    pulseError,
    isPending,
    onRetry,
}, ref) => {
    const [selectedTab, setSelectedTab] = useState<string>('');
    const [tooltipState, setTooltipState] = useState<TooltipState | null>(null);
    const innerBoxRef = useRef<HTMLDivElement>(null);

    // Keep selectedTab in sync when new results arrive (new zip fetch)
    const prevResultsRef = useRef<PulseResponse | null>(null);
    if (allResults !== prevResultsRef.current) {
        prevResultsRef.current = allResults;
        if (allResults) {
            setSelectedTab(Object.keys(allResults.byPropertyType)[0] ?? '');
        }
    }

    // Derive the active tab without waiting for a state update cycle —
    // ensures correct height on first render when reopening with cached results.
    const activeTab = selectedTab || (allResults ? Object.keys(allResults.byPropertyType)[0] ?? '' : '');

    useEffect(() => {
        if (!pulseError) return;
        const timer = setTimeout(() => onClose(), 10000);
        return () => clearTimeout(timer);
    }, [pulseError, onClose]);

    // Clear tooltip on tab switch (hovered element is replaced)
    useEffect(() => { setTooltipState(null); }, [selectedTab]);

    const handleIconEnter = (e: React.MouseEvent<HTMLSpanElement>, item: MarketDataItem) => {
        if (!innerBoxRef.current || !item.detail) return;
        const iconRect = e.currentTarget.getBoundingClientRect();
        const boxRect = innerBoxRef.current.getBoundingClientRect();
        // bottom = distance from box's bottom to 6px above the icon's top
        const bottomPx = boxRect.bottom - iconRect.top + 6;
        const side = (iconRect.left - boxRect.left) < boxRect.width / 2 ? 'left' : 'right';
        setTooltipState({ key: item.key, detail: item.detail, bottomPx, side });
    };

    const renderInnerContent = () => {
        if (isPending) {
            return (
                <div className='zc-loading-text'>
                    <Spinner width={16} height={16} />
                    <span>Processing Information</span>
                </div>
            );
        }

        if (pulseError) {
            const { text, canRetry } = getErrorDisplay(pulseError, zipCodeValue);
            return (
                <div className='zc-error-state'>
                    <MdErrorOutline className='zc-error-icon' />
                    <span className='zc-error-text'>{text}</span>
                    {canRetry && (
                        <button className='zc-retry-btn' onClick={() => onRetry()}>
                            <LuRefreshCw size={12} />
                            <span>Try again</span>
                        </button>
                    )}
                    <Minus size={16} className='zc-collapse-icon' onClick={onClose} />
                </div>
            );
        }

        if (allResults && activeTab) {
            const propertyTabs = Object.entries(allResults.byPropertyType);
            const currentData = allResults.byPropertyType[activeTab];
            return (
                <div className='zc-values-found'>
                    <div className='zc-location-block'>
                        <span className='zc-location-city'>{allResults.location.city}, {allResults.location.state} - {allResults.location.zipCode}</span>
                    </div>
						<div className='zc-property-tab-bar'>
							{propertyTabs.map(([key, typeData]) => (
								<button
									key={key}
									className={`zc-property-tab ${activeTab === key ? 'selected' : ''}`}
									onClick={() => setSelectedTab(key)}
								>
									<span>{typeData.type}</span>
								</button>
							))}
						</div>

                    <div className='zc-facts-grid'>
                        {currentData.data.filter(item => item.display).map((item) => (
                            <div key={item.key} className='zc-fact'>
                                <span className='zc-fact-bullet'>•</span>
                                <span className='zc-fact-text'>
									{item.label}
									{': '}
                                    {renderValue(item)}
                                    <span className='zc-fact-label'>
                                        {item.detail && (
                                            <span
                                                className='zc-info-icon-wrapper'
                                                onMouseEnter={(e) => handleIconEnter(e, item)}
                                                onMouseLeave={() => setTooltipState(null)}
                                            >
                                                <Info size={12} className='zc-info-icon' />
                                            </span>
                                        )}
                                    </span>
                                </span>
                            </div>
                        ))}
                    </div>

                    {allResults.disclaimer && (
                        <p className='zc-disclaimer'>{allResults.disclaimer}</p>
                    )}

                    <Minus size={16} className='zc-collapse-icon' onClick={onClose} />
                </div>
            );
        }

        return (
            <div className='zc-loading-text'>
                <Spinner width={16} height={16} />
                <span>Processing Information</span>
            </div>
        );
    };

    return (
        <div className='zip-code-dropdown' ref={ref}>
            <div className='zip-code-inner-box' ref={innerBoxRef}>
                {renderInnerContent()}
                {tooltipState && (
                    <span
                        className='zc-tooltip'
                        style={{
                            bottom: tooltipState.bottomPx,
                            ...(tooltipState.side === 'left' ? { left: 0 } : { right: 0 }),
                        }}
                    >
                        {tooltipState.detail}
                    </span>
                )}
            </div>
        </div>
    );
});

ZipCodeDropdown.displayName = 'ZipCodeDropdown';

export default ZipCodeDropdown;
