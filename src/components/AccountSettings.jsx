import { useState } from 'react';
import { LuUser, LuFileScan, LuBuilding, LuUnplug } from 'react-icons/lu';

import AccountSettingsFiles from './AccountSettingsFiles';
import AccountSettingsConnections from './AccountSettingsConnections';
import FormLineAccount from './FormLineAccount';
import SaysoModal from './SaysoModal';
import AccountSettingsCompany from './AccountSettingsCompany';

import { useAuth } from '../context/AuthContext';

import '../styles/AccountSettings.css';

export default function AccountSettings() {

    //STATE
    const [selectedPanel, setSelectedPanel] = useState('personal');
    const [nextPanel, setNextPanel] = useState('');
    const [unsavedChanges, setUnsavedChanges] = useState(false);
    const [showUnsavedChangesModal, setShowUnsavedChangesModal] = useState(false);

    //HOOKS
    const { globalUser } = useAuth();

    //FUNCTIONS
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
        <div className="account-settings-main">
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
                    <div className='account-settings-selector-container' data-view={selectedPanel}>
                        <div className={`account-settings-view-toggle-button ${selectedPanel === 'personal' ? 'active' : ''}`} onClick={() => handleSelectPanel('personal')}>
                            <LuUser />
                            <p>My Information</p>
                        </div>
                        <div className={`account-settings-view-toggle-button ${selectedPanel === 'company' ? 'active' : ''}`} onClick={() => handleSelectPanel('company')}>
                            <LuBuilding />
                            <p>My Company</p>
                        </div>
                        <div className={`account-settings-view-toggle-button ${selectedPanel === 'files' ? 'active' : ''}`} onClick={() => handleSelectPanel('files')}>
                            <LuFileScan />
                            <p>Context Files</p>
                        </div>
                        <div className={`account-settings-view-toggle-button ${selectedPanel === 'connections' ? 'active' : ''}`} onClick={() => handleSelectPanel('connections')}>
                            <LuUnplug />
                            <p>Connections</p>
                        </div>
                    </div>
                    <div className='account-settings-panel-container'>
                       <div className='account-settings-title-container'>
                            <h1>{
                                selectedPanel === 'personal' ? 'My Information' :
                                selectedPanel === 'company' ? 'My Company' :
                                selectedPanel === 'files' ? 'Context Files' :
                                selectedPanel === 'connections' && 'Connections' 
                            }</h1>
                            <p>{
                                selectedPanel === 'personal' ? 'Fill in general info about yourself' :
                                selectedPanel === 'company' ? 'Manage your company and team information' :
                                selectedPanel === 'files' ? 'Upload files to give your coach more context about your company or product' :
                                selectedPanel === 'connections' && 'Connect your software and external accounts' 
                            }</p>
                       </div>
                       <div className='account-settings-panel-container-content'>
                            {
                                selectedPanel === 'personal' ? (
                                    <div className='account-settings-personal-container'>
                                        <form className='account-settings-personal-form'>
                                            <FormLineAccount
                                                label='Name'
                                                name='name'
                                                placeholder='Name'
                                                value={globalUser.name}
                                                editable={true}
                                                setUnsavedChanges={setUnsavedChanges}
                                            />
                                            <FormLineAccount
                                                label='Lastname'
                                                name='lastname'
                                                placeholder='Lastname'
                                                value={globalUser.lastname}
                                                editable={true}
                                                setUnsavedChanges={setUnsavedChanges}
                                            />
                                            <FormLineAccount
                                                label='Email'
                                                name='email'
                                                placeholder='Email'
                                                value={globalUser.email}
                                                editable={false}
                                                setUnsavedChanges={setUnsavedChanges}
                                            />
                                        </form>
                                    </div>
                                ) :
                                selectedPanel === 'company' ? (
                                    <AccountSettingsCompany  setUnsavedChanges={setUnsavedChanges} globalUser={globalUser} />
                                ) : 
                                selectedPanel === 'files' ? (
                                    <AccountSettingsFiles />
                                ) : 
                                selectedPanel === 'connections' && (
                                    <AccountSettingsConnections />
                                ) 
                            }
                       </div>
                    </div>
                </div>
            </div>
        </div>
    );
}