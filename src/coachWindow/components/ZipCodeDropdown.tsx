import { forwardRef } from 'react';
import { Minus, MoveUpRight, MoveDownRight, Equal } from 'lucide-react';
import { Spinner } from '@/components/ui/spinner';
import { MdErrorOutline } from 'react-icons/md';
import { LuRefreshCw } from 'react-icons/lu';
import { MarketProperty, PulseApiError } from '../services/getPulseMarketProperty';

const PROPERTY_OPTIONS = [
    { key: 'sfr', value: 'SFR' },
    { key: 'multifamily', value: 'Multifamily' },
    { key: 'townhouse/condo', value: 'Townhouse/Condo' },
    { key: 'manufactured', value: 'Manufactured' },
]

const TREND_CONFIG = {
    up:      { Icon: MoveUpRight,   className: 'zc-trend-up' },
    down:    { Icon: MoveDownRight,  className: 'zc-trend-down' },
    neutral: { Icon: Equal,          className: 'zc-trend-neutral' },
} as const;

function renderTrend(trend?: { direction: 'up' | 'down' | 'neutral'; changePercent: number }) {
    if (!trend) return null;
    const { Icon, className } = TREND_CONFIG[trend.direction];
    return (
        <span className={`zc-trend ${className}`}>
            <Icon size={14} className='zc-trend-icon' />
            <span className='zc-trend-percent'>{trend.changePercent}%</span>
        </span>
    );
}

function getErrorDisplay(error: PulseApiError, zip: string): { text: string; canRetry: boolean } {
    if (error.status === 404) return { text: `No market data available for ZIP ${zip}.`, canRetry: false };
    if (error.status === 502) return { text: 'Pulse data is temporarily unavailable.', canRetry: true };
    return { text: 'Something went wrong.', canRetry: true };
}

interface Props {
    onClose: () => void;
    zipCodeValue: string;
    selectedPropertyType: string;
    setSelectedPropertyType: (v: string) => void;
    valuesFound: MarketProperty | null;
    pulseError: PulseApiError | null;
    isPending: boolean;
    onFetch: () => void;
}

const ZipCodeDropdown = forwardRef<HTMLDivElement, Props>(({
    onClose,
    zipCodeValue,
    selectedPropertyType,
    setSelectedPropertyType,
    valuesFound,
    pulseError,
    isPending,
    onFetch,
}, ref) => {
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
                        <button className='zc-retry-btn' onClick={() => onFetch()}>
                            <LuRefreshCw size={12} />
                            <span>Try again</span>
                        </button>
                    )}
                    <Minus size={16} className='zc-collapse-icon' onClick={onClose} />
                </div>
            );
        }

        if (valuesFound) {
            return (
                <div className='zc-values-found'>
                    <div className='zc-location-block'>
                        <span className='zc-location-zip'>{valuesFound.property.location.zipCode}</span>
                        <span className='zc-location-city'>{valuesFound.property.location.city}, {valuesFound.property.location.state}</span>
                    </div>
                    <div className='zc-facts-grid'>
                        {valuesFound.data.map(({ key, value, label, trend }) => (
                            <div key={key} className='zc-fact'>
                                <span className='zc-fact-bullet'>•</span>
                                <span className='zc-fact-text'>
                                    <span className='zc-fact-label'>{label} </span>
                                    <span className='zc-fact-value'>{value}</span>
                                    {renderTrend(trend)}
                                </span>
                            </div>
                        ))}
                    </div>
                    <Minus size={16} className='zc-collapse-icon' onClick={onClose} />
                </div>
            );
        }

        return (
            <>
                <div className='zc-select-property-header'>
                    <span className='zc-select-property-label'>Select Property Type</span>
                    <Minus size={16} className='zc-header-collapse-icon' onClick={onClose} />
                </div>

                <div className='zc-selectors-container'>
                    {PROPERTY_OPTIONS.map(({ key, value }) => {
                        const isSelected = selectedPropertyType === key;
                        return (
                            <div
                                key={key}
                                className={`property-type-pill ${isSelected ? 'selected' : ''}`}
                                onClick={() => setSelectedPropertyType(key)}
                            >
                                <span>{value}</span>
                            </div>
                        );
                    })}
                    <button
                        disabled={!selectedPropertyType || isPending}
                        className={`init-property-search-btn ${!selectedPropertyType ? 'disabled' : ''}`}
                        onClick={() => onFetch()}
                    >
                        Done
                    </button>
                </div>
            </>
        );
    };

    return (
        <div className='zip-code-dropdown' ref={ref}>
            <div className='zip-code-inner-box'>
                {renderInnerContent()}
            </div>
        </div>
    );
});

ZipCodeDropdown.displayName = 'ZipCodeDropdown';

export default ZipCodeDropdown;
