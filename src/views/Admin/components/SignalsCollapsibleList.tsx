import { LuSearch } from "react-icons/lu";
import { Signal } from "../types";
import SignalCollapsible from "./SignalCollapsible";
import ButtonSpinner from "@/components/ButtonSpinner";
import { useMemo, useState } from "react";
import SaysoInputGroup from "@/components/forms/SaysoInputGroup";
import { useAdminStore } from "@/store/adminStore";

interface Props {
    isLoadingSignals: boolean;
    error?: Error | null;
    refetchSignals: () => void;
    isRefetchingSignals: boolean;
}

export default function SignalsCollapsibleList({ isLoadingSignals = false, error = null, refetchSignals, isRefetchingSignals = false }: Props) {
    const [expandedSignalId, setExpandedSignalId] = useState<string[] | null>(null);
    const [searchByNameInput, setSearchByNameInput] = useState('');

    const signalSheets = useAdminStore(state => state.signalSheets);
    const activeSheetVersion = useAdminStore(state => state.activeSheetVersion);
    const liveSheetVersion = useAdminStore(state => state.liveSheetVersion);

    const displayingSignals: Signal[] = useMemo(()=>{
        const matchedSignal = signalSheets.filter(s => s.version === activeSheetVersion);
        if (!matchedSignal[0]?.signals) return [];
        return matchedSignal[0].signals;
    },[signalSheets,activeSheetVersion])

    const filteredSignals = useMemo(() => {
        if (!searchByNameInput) {``
            return displayingSignals;
        }
        const searchTerm = searchByNameInput.toLowerCase();
        return displayingSignals.filter((signal) =>
            signal.name.toLowerCase().includes(searchTerm) ||
            signal.description.toLowerCase().includes(searchTerm) ||
            signal.instructions.toLowerCase().includes(searchTerm)
        );
    }, [displayingSignals, searchByNameInput]);

    const handleExpandSignal = (id: string) => {
        setExpandedSignalId((prev) => {
            if (prev?.includes(id)) {
                return prev.filter((i) => i !== id);
            }
            return [...(prev || []), id];
        });
    };

    if (isLoadingSignals || isRefetchingSignals) {
        return (
            <div className='signals-draggable-list'>
                <div className="signals-loading-state">
                    <ButtonSpinner color="#1d4871" size={20} />
                    <span className="signals-loading-text">Loading signals...</span>
                </div>
            </div>
        );
    }

    if (error) {
        return (
            <div className='signals-draggable-list'>
                <div className="signals-error-state">
                    <span className="signals-error-title">Error Loading Signals</span>
                    <button className="sayso-outlined-button" onClick={refetchSignals} disabled={isRefetchingSignals}>
                        Retry
                    </button>
                </div>
            </div>
        );
    }

    return (
        <div className='signals-draggable-list'>
            <div className="signals-draggable-list-header">
                <div>
                    <SaysoInputGroup
                        placeholder='Search signals by name or content'
                        value={searchByNameInput}
                        onChange={(e) => setSearchByNameInput(e.target.value)}
                        icon={<LuSearch />}
                        size={40}
                    />
                </div>

                <div className='live-indicator'>
                    <span>Live</span>
                    <div className={`${liveSheetVersion === activeSheetVersion ? 'live-dot' : 'not-live-dot'}`} />
                </div>
            </div>

            {filteredSignals?.map((signal: Signal) => (
                <SignalCollapsible
                    key={signal.id}
                    signal={signal}
                    isExpanded={expandedSignalId?.includes(signal.id) ?? false}
                    handleExpandSignal={handleExpandSignal}
                />
            ))}
        </div>
    )
}
