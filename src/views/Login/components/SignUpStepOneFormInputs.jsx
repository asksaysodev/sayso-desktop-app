import CustomInput from '@/components/CustomInput';
import LoginBtn from '@/components/LoginBtn';
import { Controller } from 'react-hook-form';

export default function SignUpStepOneFormInputs({ control }) {
    return (
        <>
            <Controller
                name="name"
                control={control}
                render={({ field, fieldState: { error } }) => (
                    <CustomInput
                        type="text"
                        id="name"
                        name="name"
                        label="Name"
                        value={field.value}
                        onChange={field.onChange}
                        onBlur={field.onBlur}
                        error={error?.message}
                    />
                )}
            />
            <Controller
                name="lastname"
                control={control}
                render={({ field, fieldState: { error } }) => (
                    <CustomInput
                        type="text"
                        id="lastname"
                        name="lastname"
                        label="Last Name"
                        value={field.value}
                        onChange={field.onChange}
                        onBlur={field.onBlur}
                        error={error?.message}
                    />
                )}
            />
            <Controller
                name="company"
                control={control}
                render={({ field, fieldState: { error } }) => (
                    <CustomInput
                        type="text"
                        id="company"
                        name="company"
                        label="Company"
                        value={field.value}
                        onChange={field.onChange}
                        onBlur={field.onBlur}
                        error={error?.message}
                    />
                )}
            />
            <div className="formActions">
                <LoginBtn type="submit" text="Continue" />
            </div>
        </>
    )
}