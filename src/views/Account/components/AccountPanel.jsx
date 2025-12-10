import { useState } from "react";
import AccountSettingsTabs from "./AccountSettingsTabs";
import AccountActivePanelContainer from "./AccountActivePanelContainer";

export default function AccountPanel() {
    const [showUnsavedChangesModal, setShowUnsavedChangesModal] = useState(false);
    const [selectedPanel, setSelectedPanel] = useState('personal');
    const [nextPanel, setNextPanel] = useState('');
    const [unsavedChanges, setUnsavedChanges] = useState(false);

    const handleSelectPanel = (panel) => { 
        if(unsavedChanges) {
            setNextPanel(panel);
            setShowUnsavedChangesModal(true)
        } else {
            setSelectedPanel(panel);
        }
    }

    const handleModalConfirm = () => {
        setShowUnsavedChangesModal(false);
        setNextPanel('');
    }
    
    const handleModalDeny = () => {
        setShowUnsavedChangesModal(false);
        setSelectedPanel(nextPanel);
        setUnsavedChanges(false);
        setNextPanel('');
    }

    return (
        <div className="account-panel-main">
            {
                showUnsavedChangesModal && (
                    <SaysoModal 
                        title="Unsaved Changes"
                        text={`You have unsaved changes. Are you sure you want to switch to another panel?`}
                        isDelete = {false}
                        primaryText="Continue Editing"
                        secondaryText="Yes, Switch"
                        onDeny={() => handleModalDeny()}
                        onConfirm={() => handleModalConfirm()}
                    />
                )
            }

            <div className='account-settings-header'>
                <h1>Account Settings</h1>
            </div>

            <div className='account-settings-body'>
                <div className='account-settings-panel-container-main'>
                    <AccountSettingsTabs onSelectPanel={handleSelectPanel} selectedPanel={selectedPanel} />
                    <AccountActivePanelContainer selectedPanel={selectedPanel} setUnsavedChanges={setUnsavedChanges} />
                </div>
            </div>
        </div>
    )
}
