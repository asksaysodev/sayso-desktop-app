import CustomInput from '@/components/CustomInput';
import LoginBtn from '@/components/LoginBtn';
import PasswordInput from './PasswordInput';
import { Controller } from 'react-hook-form';

export default function LoginInFormInputs({ control, isBtnLoading }) {
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
            <div className="formActions">
                <LoginBtn type="submit" text="Sign In" isLoading={isBtnLoading} isDisabled={isBtnLoading} />
            </div>
        </>
    )
}
