import CustomInput from '@/components/CustomInput';
import LoginBtn from '@/components/LoginBtn';

export default function SignUpStepOneFormInputs({ formData, handleInputChange, handleBlur, fieldErrors }) {
    return (
        <>
            <CustomInput
                type="text"
                id="name"
                name="name"
                label="First Name"
                value={formData.name}
                onChange={handleInputChange}
                onBlur={handleBlur}
                error={fieldErrors.name}
            />
            <CustomInput
                type="text"
                id="lastname"
                name="lastname"
                label="Last Name"
                value={formData.lastname}
                onChange={handleInputChange}
                onBlur={handleBlur}
                error={fieldErrors.lastname}
            />
            <CustomInput
                type="text"
                id="company"
                name="company"
                label="Company / Brokerage"
                value={formData.company}
                onChange={handleInputChange}
                onBlur={handleBlur}
                error={fieldErrors.company}
            />
            <div className="formActions">
                <LoginBtn type="submit" text="Continue" />
            </div>
        </>
    )
}