import ViewLayout from '@/components/layouts/ViewLayout';

import './Admin.css';
import { useCallback, useMemo } from 'react';
import ToolSelector from './components/ToolSelector';
import CueMainInstructions from './components/CueMainInstructions';
import CueSignals from './components/CueSignals';
import SelectLeadType from './components/SelectLeadType';
import { useAdminStore } from '@/store/adminStore';
import { useToast } from '@/context/ToastContext';

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
const { showToast } = useToast();
    return (
       <ViewLayout title='Admin Panel'>
            <button onClick={() => showToast('success', 'Hello, world!')}>Show Toast</button>
            <div className='admin-panel-header'>
                <ToolSelector selectedTool={selectedTool} setSelectedTool={setSelectedTool} />
                {showSelectLeadType && <SelectLeadType />}
            </div>
            {renderTool()}
       </ViewLayout>
    )
}