import CustomInput from '@/components/CustomInput';
import LoginBtn from '@/components/LoginBtn';
import PasswordInput from './PasswordInput';

export default function SignUpStepTwoFormInputs({ formData, handleInputChange, handleBlur, fieldErrors, isBtnLoading, setSignupStep }) {
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
            <PasswordInput
                id="repeatPassword"
                name="repeatPassword"
                label="Repeat Password"
                value={formData.repeatPassword}
                onChange={handleInputChange}
                onBlur={handleBlur}
                error={fieldErrors.repeatPassword}
            />
            <div className="formActions">
                <LoginBtn type="submit" text="Sign Up" isLoading={isBtnLoading} isDisabled={isBtnLoading} />
                <div className='mt-20'>
                <LoginBtn type="button" text="Back" onClick={() => setSignupStep(1)} isSecondary={true} />
                </div>
            </div>
        </>
    )
}