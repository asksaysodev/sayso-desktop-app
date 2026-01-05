import CustomInput from '@/components/CustomInput';
import LoginBtn from '@/components/LoginBtn';
import PasswordInput from './PasswordInput';
import { Controller } from 'react-hook-form';

export default function SignUpStepTwoFormInputs({ control, isBtnLoading, setSignupStep }) {
    return (
        <>
            <Controller
                name="email"
                control={control}
                render={({ field, fieldState: { error } }) => (
                    <CustomInput
                        type="email"
                        id="email"
                        name="email"
                        label="Email"
                        value={field.value}
                        onChange={field.onChange}
                        onBlur={field.onBlur}
                        error={error?.message}
                    />
                )}
            />
            <Controller
                name="password"
                control={control}
                render={({ field, fieldState: { error } }) => (
                    <PasswordInput
                        id="password"
                        name="password"
                        label="Password"
                        value={field.value}
                        onChange={field.onChange}
                        onBlur={field.onBlur}
                        error={error?.message}
                    />
                )}
            />
            <Controller
                name="repeatPassword"
                control={control}
                render={({ field, fieldState: { error } }) => (
                    <PasswordInput
                        id="repeatPassword"
                        name="repeatPassword"
                        label="Repeat Password"
                        value={field.value}
                        onChange={field.onChange}
                        onBlur={field.onBlur}
                        error={error?.message}
                    />
                )}
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