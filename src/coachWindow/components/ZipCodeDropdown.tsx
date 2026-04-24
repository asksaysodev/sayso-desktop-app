import { forwardRef, useState } from 'react';
import { Minus } from 'lucide-react';
import { Spinner } from '@/components/ui/spinner';

const PROPERTY_OPTIONS = [
    {key: 'sfr', value: 'SFR'},
    {key: 'multifamily', value: 'Multifamily'},
    {key: 'townhouse/condo', value: 'Townhouse/Condo'},
    {key: 'manufactured', value: 'Manufactured'},
]

const RESULT_MOCKED = {
    zip: '33331',
    cityState: 'Miami, FL',
    facts: [
        { key: 'Average Days on Market of zip:', value: '90' },
        { key: 'Last 90 days price trend:', value: 'Up' },
        { key: 'Average Price ft2 of zip:', value: '$120,00' },
        { key: 'Inventory level:', value: 'Low' },
    ],
}

interface Props {
    onClose: () => void;
}

const ZipCodeDropdown = forwardRef<HTMLDivElement, Props>(({ onClose }, ref) => {
    const [selectedPropertyType, setSelectedPropertyType] = useState<string>("")
    const [isProcessingInformation, setIsProcessingInformation] = useState<boolean>(false);
    const [valuesFound, setValuesFound] = useState(RESULT_MOCKED)

    const renderInnerContent = () => {
        if (isProcessingInformation) {
            return (
                <div className='zc-loading-text'>
                    <Spinner width={16} height={16} />
                    <span>Processing Information</span>
                </div>
            );
        }

        if (valuesFound) {
            return (
                <div className='zc-values-found'>
                    <div className='zc-location-block'>
                        <span className='zc-location-zip'>{valuesFound.zip}</span>
                        <span className='zc-location-city'>{valuesFound.cityState}</span>
                    </div>
                    <div className='zc-facts-grid'>
                        {valuesFound.facts.map(({ key, value }) => (
                            <div key={key} className='zc-fact'>
                                <span className='zc-fact-bullet'>•</span>
                                <span className='zc-fact-text'>
                                    <span className='zc-fact-label'>{key} </span>
                                    <span className='zc-fact-value'>{value}</span>
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
                    <Minus size={16} className='zc-collapse-icon' onClick={onClose} />
                </div>

                <div className='zc-selectors-container'>
                    {PROPERTY_OPTIONS.map(({ key, value }) => {
                        const isSelected = selectedPropertyType && selectedPropertyType === key;
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
                        disabled={!selectedPropertyType}
                        className={`init-property-search-btn ${!selectedPropertyType ? 'disabled' : ''}`}
                        onClick={() => console.log('wrecked')}
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
