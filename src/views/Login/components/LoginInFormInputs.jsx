import CustomInput from '@/components/CustomInput';
import LoginBtn from '@/components/LoginBtn';

export default function LoginInFormInputs({ formData, handleInputChange, fieldErrors, isBtnLoading }) {
    return (
        <>
            <CustomInput
                type="email"
                id="email"
                name="email"
                label="Email"
                value={formData.email}
                onChange={handleInputChange}
                error={fieldErrors.email}
            />
            <CustomInput
                type="password"
                id="password"
                name="password"
                label="Password"
                value={formData.password}
                onChange={handleInputChange}
                error={fieldErrors.password}
            />
            <div className="formActions">
                <LoginBtn type="submit" text="Sign In" isLoading={isBtnLoading} isDisabled={isBtnLoading} />
            </div>
        </>
    )
}