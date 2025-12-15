import CustomInput from '@/components/CustomInput';
import LoginBtn from '@/components/LoginBtn';
import PasswordInput from './PasswordInput';

export default function LoginInFormInputs({ formData, handleInputChange, handleBlur, fieldErrors, isBtnLoading }) {
    return (
        <>
            <CustomInput
                type="email"
                id="email"
                name="email"
                label="Email"
                value={formData.email}
                onChange={handleInputChange}
                onBlur={handleBlur}
                error={fieldErrors.email}
            />
            <PasswordInput
                id="password"
                name="password"
                label="Password"
                value={formData.password}
                onChange={handleInputChange}
                onBlur={handleBlur}
                error={fieldErrors.password}
            />
            <div className="formActions">
                <LoginBtn type="submit" text="Sign In" isLoading={isBtnLoading} isDisabled={isBtnLoading} />
            </div>
        </>
    )
}
