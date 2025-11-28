import { useCoachWindowStore } from "../../store/coachWindowStore";
import Dropdown from "../../components/Dropdown";

const LEAD_TYPES = [
    { id: 'buyer', label: 'Buyer', initials: 'B' },
    { id: 'seller', label: 'Seller', initials: 'S' }
];

export default function SelectLeadTypeDropdown({ isDropdownOpen, setIsDropdownOpen }) {
    const isCoachActive = useCoachWindowStore(state => state.isCoachActive);
    const leadType = useCoachWindowStore(state => state.cue.leadType);
    const setLeadType = useCoachWindowStore(state => state.setLeadType);

    const selectedLeadType = LEAD_TYPES.find(type => type.id === leadType);

    const handleSelect = (type) => {
        setLeadType(type.id);
    };

    return (
        <Dropdown
            value={selectedLeadType}
            items={LEAD_TYPES}
            onChange={handleSelect}
            isOpen={isDropdownOpen}
            setIsOpen={setIsDropdownOpen}
            placeholder="Select Lead Type"
            disabled={isCoachActive}
        />
    );
}
