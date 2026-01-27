import { useEffect } from "react";
import { useCoachWindowStore } from "../../store/coachWindowStore";
import Dropdown from "../../components/Dropdown";

interface SelectProspectDropdownProps {
    isDropdownOpen: boolean;
    setIsDropdownOpen: (isOpen: boolean) => void;
}

export default function SelectProspectDropdown({ isDropdownOpen, setIsDropdownOpen }: SelectProspectDropdownProps) {
    const isCoachActive = useCoachWindowStore(state => state.isCoachActive);
    const selectedProspect = useCoachWindowStore(state => state.recall.selectedProspect);
    const setSelectedProspect = useCoachWindowStore(state => state.setSelectedProspect);
    const fetchProspects = useCoachWindowStore(state => state.recall_fetchProspects);
    const prospects = useCoachWindowStore(state => state.recall.prospects);
    const isLoadingProspects = useCoachWindowStore(state => state.recall.isLoadingProspects);
    const prospectsError = useCoachWindowStore(state => state.recall.prospectsError);

    useEffect(() => {
        // For now i will leave it as it was before, but
        // we need to be sure that the prospects dont get stale
        fetchProspects();
    }, []);

    return (
        <Dropdown
            value={selectedProspect}
            items={prospects}
            onChange={setSelectedProspect}
            isOpen={isDropdownOpen}
            setIsOpen={setIsDropdownOpen}
            placeholder="Select Prospect"
            disabled={isCoachActive}
            searchable
            searchPlaceholder="Search Prospects"
            getLabel={(prospect) => `${prospect.name} ${prospect.lastname}`}
            getDescription={(prospect) => prospect.email}
            getInitials={(prospect) => 
                prospect.name.charAt(0).toUpperCase() + 
                prospect.lastname.charAt(0).toUpperCase()
            }
            searchKeys={['name', 'lastname', 'email', 'company']}
            isLoading={isLoadingProspects}
            error={prospectsError}
            emptyMessage="No prospects found"
        />
    );
}
