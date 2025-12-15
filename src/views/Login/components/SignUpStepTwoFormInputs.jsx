import CustomInput from '@/components/CustomInput';
import LoginBtn from '@/components/LoginBtn';

export default function SignUpStepTwoFormInputs({ formData, handleInputChange, fieldErrors, isBtnLoading, setSignupStep }) {
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
            <CustomInput
                type="password"
                id="repeatPassword"
                name="repeatPassword"
                label="Repeat Password"
                value={formData.repeatPassword}
                onChange={handleInputChange}
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