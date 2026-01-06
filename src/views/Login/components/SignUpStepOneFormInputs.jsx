import CustomInput from '@/components/CustomInput';
import LoginBtn from '@/components/LoginBtn';

export default function SignUpStepOneFormInputs({ control }) {
    return (
        <>
            <CustomInput
                type="text"
                id="name"
                name="name"
                label="Name"
                control={control}
            />
            <CustomInput
                type="text"
                id="lastname"
                name="lastname"
                label="Last Name"
                control={control}
            />
            <CustomInput
                type="text"
                id="company"
                name="company"
                label="Company"
                control={control}
            />
            <div className="formActions">
                <LoginBtn type="submit" text="Continue" />
            </div>
        </>
    )
}