import LoginBtn from '@/components/LoginBtn';
import ControlledInputField from '@/components/forms/ControlledInputField';

export default function SignUpStepOneFormInputs({ control }) {
    return (
        <div className='flex flex-col gap-5'>
            <ControlledInputField
                control={control}
                name="name"
                label="Name"
                labelCn='loginInFormInputLabel'
            />
            <ControlledInputField
                control={control}
                name="lastname"
                label="Last Name"
                labelCn='loginInFormInputLabel'
            />
            <ControlledInputField
                control={control}
                name="company"
                label="Company"
                labelCn='loginInFormInputLabel'
            />
            <div className="formActions">
                <LoginBtn type="submit" text="Continue" />
            </div>
        </div>
    )
}