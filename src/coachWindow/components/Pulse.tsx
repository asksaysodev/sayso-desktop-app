import { CoachWindowStore } from "@/types/store/coachWindowStore";
import { SetStateAction } from "react";
import { LuChevronDown, LuChevronUp, LuSearch } from "react-icons/lu";

interface Props {
    isCoachActive: CoachWindowStore['isCoachActive'];
    isPulseEnabled: boolean;
    isZipCodeValid: boolean;
    setIsZipDropdownOpen: React.Dispatch<SetStateAction<boolean>>;
    isZipDropdownOpen: boolean;
    setZipCodeValue: React.Dispatch<SetStateAction<string>>;
    zipCodeValue: string;
    /** Shown but not usable, e.g. while the session resets (SAYSO-566). */
    disabled?: boolean;
}

export default function Pulse({ isCoachActive, isPulseEnabled, isZipCodeValid, setIsZipDropdownOpen, isZipDropdownOpen, setZipCodeValue, zipCodeValue, disabled = false }: Props) {

    if (!isCoachActive || !isPulseEnabled) return null;

	return (
        <div
            className={`zip-code-input-container ${isZipCodeValid ? 'active' : ''} ${disabled ? 'disabled' : ''}`}
            onClick={() => {
                if (isZipCodeValid && !disabled) setIsZipDropdownOpen(true);
            }}
        >
            <LuSearch className='zip-code-input-icon' />
            <input
                className={`zip-code-input ${isZipCodeValid ? 'valid' : ''}`}
                placeholder='Zip Code'
                value={zipCodeValue}
                onChange={(e) => setZipCodeValue(e.target.value.replace(/\D/g, '').slice(0, 5))}
                inputMode='numeric'
                pattern='[0-9]{5}'
                maxLength={5}
                disabled={disabled}
            />
            {isZipCodeValid && (
                <button
                    type='button'
                    className='zip-code-chevron-toggle'
                    disabled={disabled}
                    onClick={(e) => {
                        e.stopPropagation();
                        setIsZipDropdownOpen(prev => !prev);
                    }}
                    aria-label={isZipDropdownOpen ? 'Collapse zip details' : 'Expand zip details'}
                >
                    {isZipDropdownOpen ? <LuChevronUp /> : <LuChevronDown />}
                </button>
            )}
        </div>
	)
}