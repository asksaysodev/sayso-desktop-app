import FormLineAccount from "../../../components/FormLineAccount"
import { useAuth } from "../../../context/AuthContext";
import AccountSettingsCompanyForm from "./AccountSettingsCompanyForm"
import AccountSettingsConnectionsForm from "./AccountSettingsConnectionsForm"
import AccountSettingsFilesForm from "./AccountSettingsFilesForm"
import AccountSettingsPersonalForm from "./AccountSettingsPersonalForm";

export default function AccountActivePanelContainer({ selectedPanel, setUnsavedChanges }) {

    const { globalUser } = useAuth();

    const panelFormContent = {
        'personal': {
            title: 'My Information',
            description: 'Fill in general info about yourself'
        },
        'company': {
            title: 'My Company',
            description: 'Manage your company and team information'
        },
        'files': {
            title: 'Context Files',
            description: 'Upload files to give your coach more context about your company or product'
        },
        'connections': {
            title: 'Connections',
            description: 'Connect your software and external accounts'
        },
    }

    return (
        <div className='account-settings-panel-container'>
            <div className='account-settings-title-container'>
                <h1>{panelFormContent[selectedPanel]?.title}</h1>
                <p>{panelFormContent[selectedPanel]?.description}</p>
            </div>
            <div className='account-settings-panel-container-content'>
                 {
                    selectedPanel === 'personal' ? (
                       <AccountSettingsPersonalForm setUnsavedChanges={setUnsavedChanges} globalUser={globalUser} />
                    ) :
                    selectedPanel === 'company' ? (
                        <AccountSettingsCompanyForm  setUnsavedChanges={setUnsavedChanges} globalUser={globalUser} />
                    ) : 
                    selectedPanel === 'files' ? (
                        <AccountSettingsFilesForm />
                    ) : 
                    selectedPanel === 'connections' && (
                        <AccountSettingsConnectionsForm />
                    ) 
                }
            </div>
        </div>
    )
}