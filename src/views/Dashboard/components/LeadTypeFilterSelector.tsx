import { Checkbox } from "@/components/ui/checkbox";

const LeadTypeOptions = [
    { key: 'all', label: 'All'},
    { key: 'buyer', label: 'Buyer' },
    { key:'seller',label:'Seller' }
] as const;

export default function LeadTypeFilterSelector({ selectedLeadTypeFilter, setSelectedLeadTypeFilter }) {
    return (
        <div className="lead-type-filter-selector-container">
            {LeadTypeOptions.map(({ key, label }) => {
                const isActive: boolean = selectedLeadTypeFilter === key;

                return (
                    <div
                        key={key}
                        onClick={() => setSelectedLeadTypeFilter(key)}
                        className="lead-type-option"
                        style={{ backgroundColor: isActive ? 'var(--sayso-white)' : undefined }}
                    >
                        <Checkbox checked={isActive} className="lead-type-checkbox"/>
                        {label}
                    </div>
                )
            })}
        </div>
    )
}