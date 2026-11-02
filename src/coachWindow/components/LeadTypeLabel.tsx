import { useCoachWindowStore } from "../../store/coachWindowStore";
import { LeadTypeEnum } from "@/types/coach";

const LEAD_TYPE_LABELS: Record<LeadTypeEnum, string> = {
    [LeadTypeEnum.BUYER]: 'Buyer',
    [LeadTypeEnum.SELLER]: 'Seller',
};

// The active session's lead type, read-only (SAYSO-566): the lead-type dropdown
// is gone, and the lead is picked by the Start button itself. The label pill of
// the dropdown's locked state in production.
export default function LeadTypeLabel() {
    const isCoachActive = useCoachWindowStore(state => state.isCoachActive);
    const leadType = useCoachWindowStore(state => state.cue.leadType);

    if (!isCoachActive || !leadType) return null;
    const label = LEAD_TYPE_LABELS[leadType];

    return (
        <div className="lead-type-label" aria-label={`Lead type: ${label}`}>
            {label}
        </div>
    );
}
