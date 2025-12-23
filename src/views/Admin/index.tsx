import ViewLayout from '@/components/layouts/ViewLayout';

import './Admin.css';
import { useCallback, useMemo } from 'react';
import ToolSelector from './components/ToolSelector';
import CueMainInstructions from './components/CueMainInstructions';
import CueSignals from './components/CueSignals';
import SelectLeadType from './components/SelectLeadType';
import { useAdminStore } from '@/store/adminStore';

export default function Admin() {
    const selectedTool = useAdminStore(state => state.selectedTool);
    const setSelectedTool = useAdminStore(state => state.setSelectedTool);
    
    const renderTool = useCallback(() => {
        if (selectedTool === 'cue-signals') {
            return <CueSignals />
        }
        return <CueMainInstructions />
    }, [selectedTool]);

    const showSelectLeadType = useMemo(() => ['cue-signals', 'cue-main-instructions'].includes(selectedTool), [selectedTool]);

    return (
       <ViewLayout title='Admin Panel'>
            <div className='admin-panel-header'>
                <ToolSelector selectedTool={selectedTool} setSelectedTool={setSelectedTool} />
                {showSelectLeadType && <SelectLeadType />}
            </div>
            {renderTool()}
       </ViewLayout>
    )
}