import CustomInput from '@/components/CustomInput';
import LoginBtn from '@/components/LoginBtn';
import PasswordInput from './PasswordInput';

export default function LoginInFormInputs({ control, isBtnLoading }) {
    return (
        <>
            <CustomInput
                type="email"
                id="email"
                name="email"
                label="Email"
                control={control}
            />
            <PasswordInput
                id="password"
                name="password"
                label="Password"
                control={control}
            />
            <div className="formActions">
                <LoginBtn type="submit" text="Sign In" isLoading={isBtnLoading} isDisabled={isBtnLoading} />
            </div>
        </>
    )
}
