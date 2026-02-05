import { LuArrowUp } from 'react-icons/lu';

export default function ImportSheetButton() {
    return (
        <label className="import-sheet-button">
            <LuArrowUp size={16} />
            <span>Import</span>
            <input
                type="file"
                 accept=".csv,.xlsx,.xls"
            />
        </label>
    )
}