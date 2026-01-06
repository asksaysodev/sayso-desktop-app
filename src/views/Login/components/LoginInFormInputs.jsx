import CustomInput from '@/components/CustomInput';
import LoginBtn from '@/components/LoginBtn';
import PasswordInput from './PasswordInput';
import ControlledInputField from '@/components/forms/ControlledInputField';
import { useState } from 'react';
import { LuEye, LuEyeOff } from 'react-icons/lu';
import EyeToggleShowPasswordButton from './EyeToggleShowPasswordButton';

export default function LoginInFormInputs({ control, isBtnLoading }) {
    const [showPassword, setShowPassword] = useState(false);

    const togglePasswordVisibility = () => {
        setShowPassword(prev => !prev);
    };

    return (
        <div className='flex flex-col gap-5'>
            <ControlledInputField
                control={control}
                name='email'
                label='Email'
                labelCn='loginInFormInputLabel'
            />
            <ControlledInputField
                type={showPassword ? 'text' : 'password'}
                control={control}
                name='password'
                label='Password'
                labelCn='loginInFormInputLabel'
                rightChildren={<EyeToggleShowPasswordButton
                    showPassword={showPassword}
                    togglePasswordVisibility={togglePasswordVisibility}
                    />
                }
            />
            <div className="formActions">
                <LoginBtn type="submit" text="Sign In" isLoading={isBtnLoading} isDisabled={isBtnLoading} />
            </div>
        </div>
    )
}
