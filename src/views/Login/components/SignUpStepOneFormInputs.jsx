import CustomInput from '@/components/CustomInput';
import LoginBtn from '@/components/LoginBtn';

export default function SignUpStepOneFormInputs({ formData, handleInputChange, fieldErrors }) {
    return (
        <>
            <CustomInput
                type="text"
                id="name"
                name="name"
                label="Name"
                value={formData.name}
                onChange={handleInputChange}
                error={fieldErrors.name}
            />
            <CustomInput
                type="text"
                id="lastname"
                name="lastname"
                label="Last Name"
                value={formData.lastname}
                onChange={handleInputChange}
                error={fieldErrors.lastname}
            />
            <CustomInput
                type="text"
                id="company"
                name="company"
                label="Company"
                value={formData.company}
                onChange={handleInputChange}
                error={fieldErrors.company}
            />
            <div className="formActions">
                <LoginBtn type="submit" text="Continue" />
            </div>
        </>
    )
}