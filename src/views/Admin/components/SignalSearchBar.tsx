import { useState } from 'react';
import { LuSearch, LuX, LuChevronDown } from 'react-icons/lu';
import { InputGroup, InputGroupInput, InputGroupAddon } from '@/components/ui/input-group';
import {
    Popover,
    PopoverContent,
    PopoverTrigger,
} from '@/components/ui/popover';
import { SignalStageFitKey, SignalStageFitValue } from '../types';

export interface StageFitFilter {
    stage: SignalStageFitKey;
    value: SignalStageFitValue;
}

interface Props {
    searchText: string;
    onSearchTextChange: (text: string) => void;
    stageFitFilter: StageFitFilter | null;
    onStageFitFilterChange: (filter: StageFitFilter | null) => void;
}

const STAGES: SignalStageFitKey[] = ['connect', 'discover', 'convert'];
const STAGE_FIT_VALUES: SignalStageFitValue[] = ['Primary', 'Lower', 'Allowed'];

export default function SignalSearchBar({
    searchText,
    onSearchTextChange,
    stageFitFilter,
    onStageFitFilterChange
}: Props) {
    const [isFilterDropdownOpen, setIsFilterDropdownOpen] = useState(false);
    const [isStageDropdownOpen, setIsStageDropdownOpen] = useState(false);
    const [isValueDropdownOpen, setIsValueDropdownOpen] = useState(false);

    const handleSelectStageFitOption = () => {
        onStageFitFilterChange({ stage: 'connect', value: 'Primary' });
        setIsFilterDropdownOpen(false);
    };

    const handleStageChange = (stage: SignalStageFitKey) => {
        if (stageFitFilter) {
            onStageFitFilterChange({ ...stageFitFilter, stage });
        }
        setIsStageDropdownOpen(false);
    };

    const handleValueChange = (value: SignalStageFitValue) => {
        if (stageFitFilter) {
            onStageFitFilterChange({ ...stageFitFilter, value });
        }
        setIsValueDropdownOpen(false);
    };

    const handleRemoveFilter = () => {
        onStageFitFilterChange(null);
    };

    return (
        <div className="signal-search-bar">
            <Popover
                open={isFilterDropdownOpen}
                onOpenChange={setIsFilterDropdownOpen}
            >
                <PopoverTrigger asChild>
                    <div className="signal-search-bar-input-wrapper">
                        <InputGroup className="!h-10">
                            <InputGroupAddon>
                                <LuSearch size={16} />
                            </InputGroupAddon>
                            {stageFitFilter && (
                                <div className="signal-search-filter-pill">
                                    <span className="filter-pill-label">stage_fit</span>

                                    <Popover open={isStageDropdownOpen} onOpenChange={setIsStageDropdownOpen}>
                                        <PopoverTrigger asChild>
                                            <button className="filter-pill-selector" onClick={(e) => e.stopPropagation()}>
                                                {stageFitFilter.stage}
                                                <LuChevronDown size={10} />
                                            </button>
                                        </PopoverTrigger>
                                        <PopoverContent className="filter-dropdown w-28" align="start">
                                            {STAGES.map((stage) => (
                                                <button
                                                    key={stage}
                                                    className={`filter-dropdown-item ${stageFitFilter.stage === stage ? 'active' : ''}`}
                                                    onClick={() => handleStageChange(stage)}
                                                >
                                                    {stage}
                                                </button>
                                            ))}
                                        </PopoverContent>
                                    </Popover>

                                    <span className="filter-pill-is">is</span>

                                    <Popover open={isValueDropdownOpen} onOpenChange={setIsValueDropdownOpen}>
                                        <PopoverTrigger asChild>
                                            <button className="filter-pill-selector" onClick={(e) => e.stopPropagation()}>
                                                {stageFitFilter.value}
                                                <LuChevronDown size={10} />
                                            </button>
                                        </PopoverTrigger>
                                        <PopoverContent className="filter-dropdown w-28" align="start">
                                            {STAGE_FIT_VALUES.map((value) => (
                                                <button
                                                    key={value}
                                                    className={`filter-dropdown-item ${stageFitFilter.value === value ? 'active' : ''}`}
                                                    onClick={() => handleValueChange(value)}
                                                >
                                                    {value}
                                                </button>
                                            ))}
                                        </PopoverContent>
                                    </Popover>

                                    <button className="filter-pill-remove" onClick={handleRemoveFilter}>
                                        <LuX size={12} />
                                    </button>
                                </div>
                            )}
                            <InputGroupInput
                                placeholder={stageFitFilter ? 'Search...' : 'Search signals by name or content'}
                                value={searchText}
                                onChange={(e) => onSearchTextChange(e.target.value)}
                                className="!h-10"
                            />
                        </InputGroup>
                    </div>
                </PopoverTrigger>
                {!stageFitFilter && (
                    <PopoverContent className="signal-filter-options" align="start" onOpenAutoFocus={(e) => e.preventDefault()}>
                        <button className="signal-filter-option" onClick={handleSelectStageFitOption}>
                            <span className="signal-filter-option-label">stage_fit</span>
                            <span className="signal-filter-option-description">Filter by stage fit value</span>
                        </button>
                    </PopoverContent>
                )}
            </Popover>
        </div>
    );
}
