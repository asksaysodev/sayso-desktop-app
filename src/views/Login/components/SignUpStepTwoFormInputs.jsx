import CustomInput from '@/components/CustomInput';
import LoginBtn from '@/components/LoginBtn';
import PasswordInput from './PasswordInput';

export default function SignUpStepTwoFormInputs({ control, isBtnLoading, setSignupStep }) {
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
            <PasswordInput
                id="repeatPassword"
                name="repeatPassword"
                label="Repeat Password"
                control={control}
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