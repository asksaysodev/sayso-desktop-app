import { LuUser, LuFileScan, LuBuilding, LuUnplug } from 'react-icons/lu';

export default function AccountSettingsTabs({ onSelectPanel, selectedPanel }) {
  return (
    <div className='account-settings-selector-container' data-view={selectedPanel}>
        <div className={`account-settings-view-toggle-button ${selectedPanel === 'personal' ? 'active' : ''}`} onClick={() => onSelectPanel('personal')}>
            <LuUser />
            <p>My Information</p>
        </div>
        <div className={`account-settings-view-toggle-button ${selectedPanel === 'company' ? 'active' : ''}`} onClick={() => onSelectPanel('company')}>
            <LuBuilding />
            <p>My Company</p>
        </div>
        <div className={`account-settings-view-toggle-button ${selectedPanel === 'files' ? 'active' : ''}`} onClick={() => onSelectPanel('files')}>
            <LuFileScan />
            <p>Context Files</p>
        </div>
        <div className={`account-settings-view-toggle-button ${selectedPanel === 'connections' ? 'active' : ''}`} onClick={() => onSelectPanel('connections')}>
            <LuUnplug />
            <p>Connections</p>
        </div>
    </div>
  )
}