import { LuUser, LuFileScan, LuBuilding, LuUnplug } from 'react-icons/lu';
import TabSelector from '../../../components/TabSelector';
import '../../../components/TabSelector.css';

const ICON_SIZE = 18;

export default function AccountSettingsTabs({ onSelectPanel, selectedPanel }) {
    const tabs = [
        {
            icon: <LuUser size={ICON_SIZE} />,
            label: 'My Information',
            value: 'personal'
        },
        {
            icon: <LuBuilding size={ICON_SIZE} />,
            label: 'My Company',
            value: 'company'
        },
        {
            icon: <LuFileScan size={ICON_SIZE} />,
            label: 'Context Files',
            value: 'files',
            hidden: true
        },
        {
            icon: <LuUnplug size={ICON_SIZE} />,
            label: 'Connections',
            value: 'connections',
            hidden: true
        },
    ];
    
    return (
        <div className='account-settings-selector-container'>
            <TabSelector 
                tabs={tabs}
                selectedValue={selectedPanel}
                onChange={onSelectPanel}
            />
        </div>
    )
}