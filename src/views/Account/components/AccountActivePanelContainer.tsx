import FormLineAccount from "../../../components/FormLineAccount"
import { useAuth } from "../../../context/AuthContext";
import { AccountSettingsPanel, AccountSettingsPanelEnum } from "../types";
import AccountSettingsCompanyForm from "./AccountSettingsCompanyForm"
import AccountSettingsConnectionsForm from "./AccountSettingsConnectionsForm"
import AccountSettingsFilesForm from "./AccountSettingsFilesForm"
import AccountSettingsPersonalForm from "./AccountSettingsPersonalForm";

interface Props {
    selectedPanel: AccountSettingsPanel;
    setUnsavedChanges: (unsavedChanges: boolean) => void;
}

export default function AccountActivePanelContainer({ selectedPanel, setUnsavedChanges }: Props) {

    const { globalUser } = useAuth();

    const panelFormContent = {
        [AccountSettingsPanelEnum.PERSONAL]: {
            title: 'My Information',
            description: 'Fill in general info about yourself'
        },
        [AccountSettingsPanelEnum.COMPANY]: {
            title: 'My Company',
            description: 'Manage your company and team information'
        },
        [AccountSettingsPanelEnum.FILES]: {
            title: 'Context Files',
            description: 'Upload files to give your coach more context about your company or product'
        },
        [AccountSettingsPanelEnum.CONNECTIONS]: {
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
                    selectedPanel === AccountSettingsPanelEnum.PERSONAL ? (
                       <AccountSettingsPersonalForm setUnsavedChanges={setUnsavedChanges} globalUser={globalUser} />
                    ) :
                    selectedPanel === AccountSettingsPanelEnum.COMPANY ? (
                        <AccountSettingsCompanyForm  setUnsavedChanges={setUnsavedChanges} globalUser={globalUser} />
                    ) : 
                    selectedPanel === AccountSettingsPanelEnum.FILES ? (
                        <AccountSettingsFilesForm />
                    ) : 
                    selectedPanel === AccountSettingsPanelEnum.CONNECTIONS && (
                        <AccountSettingsConnectionsForm />
                    ) 
                }
            </div>
        </div>
    )
}