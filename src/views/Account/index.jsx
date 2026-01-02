import SaysoModal from "@/components/SaysoModal";
import AccountSettingsTabs from "./components/AccountSettingsTabs";
import AccountActivePanelContainer from "./components/AccountActivePanelContainer";
import { useState } from "react";
import ViewLayout from "@/components/layouts/ViewLayout";
import "./styles.css";

export default function Account() {
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
		<ViewLayout title="Account Settings">
			{
				showUnsavedChangesModal && (
					<SaysoModal
						title="Unsaved Changes"
						text={`You have unsaved changes. Are you sure you want to switch to another panel?`}
						isDelete={false}
						primaryText="Continue Editing"
						secondaryText="Yes, Switch"
						onDeny={() => handleModalDeny()}
						onConfirm={() => handleModalConfirm()}
					/>
				)
			}

			<div className='account-settings-body'>
				<div className='account-settings-panel-container-main'>
					<AccountSettingsTabs onSelectPanel={handleSelectPanel} selectedPanel={selectedPanel} />
					<AccountActivePanelContainer selectedPanel={selectedPanel} setUnsavedChanges={setUnsavedChanges} />
				</div>
			</div>  
		</ViewLayout>
	);
}
