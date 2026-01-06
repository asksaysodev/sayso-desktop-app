import LoginBtn from '@/components/LoginBtn';
import ControlledInputField from '@/components/forms/ControlledInputField';
import EyeToggleShowPasswordButton from './EyeToggleShowPasswordButton';
import { useState } from 'react';

export default function SignUpStepTwoFormInputs({ control, isBtnLoading, setSignupStep }) {
    const [showPassword, setShowPassword] = useState(false);
    const [showRepeatPassword, setShowRepeatPassword] = useState(false);

    const togglePasswordVisibility = () => {
        setShowPassword(prev => !prev);
    };

    const toggleRepeatPasswordVisibility = () => {
        setShowRepeatPassword(prev => !prev);
    };

    return (
        <div className='flex flex-col gap-5'>
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
                    togglePasswordVisibility={togglePasswordVisibility}
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
                    togglePasswordVisibility={toggleRepeatPasswordVisibility}
                />}
            />
            <div className="formActions">
                <LoginBtn type="submit" text="Sign Up" isLoading={isBtnLoading} isDisabled={isBtnLoading} />
                <LoginBtn type="button" text="Back" onClick={() => setSignupStep(1)} isSecondary={true} />
            </div>
        </div>
    )
}
