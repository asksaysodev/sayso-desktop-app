import { useState } from 'react';

import { useAuth } from '../context/AuthContext';

import { LuUser, LuFileScan, LuBuilding, LuUnplug } from 'react-icons/lu';

import FormLineAccount from './FormLineAccount';


import '../styles/AccountSettings.css';


export default function AccountSettings() {

    //STATE
    const [selectedPanel, setSelectedPanel] = useState('personal');

    //HOOKS
    const { globalUser } = useAuth();

    //FUNCTIONS
    const handleSelectPanel = (panel) => {
        setSelectedPanel(panel);
    }


    return (
        <div className="account-settings-main">
            <div className='account-settings-header'>
                <h1>Account Settings</h1>
            </div>
            <div className='account-settings-body'>
                <div className='account-settings-panel-container-main'>
                    <div className='account-settings-selector-container' data-view={selectedPanel}>
                        <div className={`account-settings-view-toggle-button ${selectedPanel === 'personal' ? 'active' : ''}`} onClick={() => setSelectedPanel('personal')}>
                            <LuUser />
                            <p>My Information</p>
                        </div>
                        <div className={`account-settings-view-toggle-button ${selectedPanel === 'company' ? 'active' : ''}`} onClick={() => setSelectedPanel('company')}>
                            <LuBuilding />
                            <p>My Company</p>
                        </div>
                        <div className={`account-settings-view-toggle-button ${selectedPanel === 'files' ? 'active' : ''}`} onClick={() => setSelectedPanel('files')}>
                            <LuFileScan />
                            <p>Context Files</p>
                        </div>
                        <div className={`account-settings-view-toggle-button ${selectedPanel === 'connections' ? 'active' : ''}`} onClick={() => setSelectedPanel('connections')}>
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
                                selectedPanel === 'connections' && 'Connect your software and external devices' 
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
                                                // onChange={onChange}
                                                // onSave={onSave}
                                                // onCancel={onCancel}
                                           />
                                        </form>
                                    </div>
                                ) :
                                selectedPanel === 'company' ? (
                                    <div className='account-settings-company-container'>
                                        <div className='account-settings-company-info'>
                                            <h2>Company Info</h2>
                                        </div>
                                    </div>
                                ) : 
                                selectedPanel === 'files' ? (
                                    <div className='account-settings-files-container'>
                                        <div className='account-settings-files-info'>
                                            <h2>Files Info</h2>
                                        </div>
                                    </div>
                                ) : 
                                selectedPanel === 'connections' && (
                                    <div className='account-settings-connections-container'>
                                        <div className='account-settings-connections-info'>
                                            <h2>Connections Info</h2>
                                        </div>
                                    </div>
                                ) 
                            }
                       </div>
                    </div>
                </div>
            </div>
        </div>
    );
}