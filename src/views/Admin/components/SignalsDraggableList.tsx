import { LuLoader } from "react-icons/lu";
import { Signal } from "../types";
import CreateSignalDialog from "./CreateSignalDialog";
import SignalDraggable from "./SignalDraggable";
import ButtonSpinner from "@/components/ButtonSpinner";

interface Props {
    signals: Signal[];
    handleOnChangeSignalField: (id: string, field: string, value: string | number) => void;
    handleDeleteSignal: (id: string) => void;
    handleSubmitAddSignal: (data: Signal) => void;
    isLoadingSignals: boolean;
    error?: Error | null;
    refetchSignals: () => void;
    isRefetchingSignals: boolean;
}

export default function SignalsDraggableList({ signals = [], handleOnChangeSignalField, handleDeleteSignal, handleSubmitAddSignal, isLoadingSignals = false, error = null, refetchSignals, isRefetchingSignals = false }: Props) {
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
            <CreateSignalDialog signalsLength={signals.length} handleSubmitAddSignal={handleSubmitAddSignal} />
            
            {signals?.map((signal: Signal) => (
                <SignalDraggable key={signal.id} signal={signal} handleOnChangeSignalField={handleOnChangeSignalField} handleDeleteSignal={handleDeleteSignal} />
            ))}
        </div>
    )
}