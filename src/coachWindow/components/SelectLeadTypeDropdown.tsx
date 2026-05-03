import { useRef, useEffect } from 'react';
import { useCoachWindowStore } from "../../store/coachWindowStore";
import Dropdown from "../../components/Dropdown";
import { LeadTypeEnum } from "@/types/coach";

const LEAD_TYPES = [
    { id: LeadTypeEnum.BUYER, label: 'Buyer', initials: 'B' },
    { id: LeadTypeEnum.SELLER, label: 'Seller', initials: 'S' }
];

interface LeadTypeItem {
    id: LeadTypeEnum;
    label: string;
    initials: string;
}

interface Props {
    isDropdownOpen: boolean;
    setIsDropdownOpen: (isOpen: boolean) => void;
}

export default function SelectLeadTypeDropdown({ isDropdownOpen, setIsDropdownOpen }: Props) {
    const isCoachActive = useCoachWindowStore(state => state.isCoachActive);
    const leadType = useCoachWindowStore(state => state.cue.leadType);
    const setLeadType = useCoachWindowStore(state => state.setLeadType);
    const wrapperRef = useRef<HTMLDivElement>(null);

    const selectedLeadType = LEAD_TYPES.find(type => type.id === leadType);

    useEffect(() => {
        if (!isDropdownOpen) return;

        const handleDocumentMouseDown = (e: MouseEvent) => {
            if (wrapperRef.current && !wrapperRef.current.contains(e.target as Node)) {
                setIsDropdownOpen(false);
            }
        };

        document.addEventListener('mousedown', handleDocumentMouseDown);
        return () => document.removeEventListener('mousedown', handleDocumentMouseDown);
    }, [isDropdownOpen]);

    const handleSelect = (leadTypeSelected: LeadTypeItem) => {
        setLeadType(leadTypeSelected.id);
        setIsDropdownOpen(false);
    };

    return (
        <div ref={wrapperRef}>
            <Dropdown
                value={selectedLeadType}
                items={LEAD_TYPES}
                onChange={handleSelect}
                isOpen={isDropdownOpen}
                setIsOpen={setIsDropdownOpen}
                placeholder="Select Lead Type"
                disabled={isCoachActive}
            />
        </div>
    );
}
