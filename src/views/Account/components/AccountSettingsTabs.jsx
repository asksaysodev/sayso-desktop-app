import { LuUser, LuFileScan, LuBuilding, LuUnplug } from 'react-icons/lu';

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
        <div className='account-settings-selector-container' data-view={selectedPanel}>
            {tabs.map(({ icon, label, value, hidden = false }) => {
                if (hidden) return null;

                return (
                    <div key={value} className={`account-settings-view-toggle-button ${selectedPanel === value ? 'active' : ''}`} onClick={() => onSelectPanel(value)}>
                        {icon}
                        <p>{label}</p>
                    </div>
                )
            })}
        </div>
    )
}