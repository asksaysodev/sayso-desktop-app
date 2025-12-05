import FormLineAccount from "../../../components/FormLineAccount";

export default function AccountSettingsPersonalForm({ setUnsavedChanges, globalUser }) {
    return (
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
    )
}