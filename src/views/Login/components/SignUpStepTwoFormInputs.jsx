import LoginBtn from '@/components/LoginBtn';
import ControlledInputField from '@/components/forms/ControlledInputField';
import EyeToggleShowPasswordButton from './EyeToggleShowPasswordButton';
import { useState } from 'react';

export default function SignUpStepTwoFormInputs({ control, isBtnLoading, setSignupStep }) {
    const [showPassword, setShowPassword] = useState(false);
    const [showRepeatPassword, setShowRepeatPassword] = useState(false);

    return (
        <div className='login-layout-form-inputs-wrapper'>
            <ControlledInputField
                type="email"
                control={control}
                name="email"
                label="Email"
                labelCn='loginInFormInputLabel'
            />
            <ControlledInputField
                type={showPassword ? 'text' : 'password'}
                control={control}
                name="password"
                label="Password"
                labelCn='loginInFormInputLabel'
                rightChildren={<EyeToggleShowPasswordButton
                    showPassword={showPassword}
                    setShowPassword={setShowPassword}
                />}
            />
            <ControlledInputField
                type={showRepeatPassword ? 'text' : 'password'}
                control={control}
                name="repeatPassword"
                label="Repeat Password"
                labelCn='loginInFormInputLabel'
                rightChildren={<EyeToggleShowPasswordButton
                    showPassword={showRepeatPassword}
                    setShowPassword={setShowRepeatPassword}
                />}
            />
            <div className="formActions">
                <LoginBtn type="submit" text="Sign Up" isLoading={isBtnLoading} isDisabled={isBtnLoading} />
                <LoginBtn type="button" text="Back" onClick={() => setSignupStep(1)} isSecondary={true} />
            </div>
        </div>
    )
}
