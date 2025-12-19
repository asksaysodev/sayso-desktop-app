import { RegisterOptions, UseFormReturn } from "react-hook-form";
import { Input } from "../ui/input";
import ControlledCustomFormField from "./ControlledCustomFormField";
import { HTMLInputTypeAttribute } from "react";

interface Props {
    name: string;
    control: UseFormReturn<any>['control'];
    rules?: RegisterOptions;
    label?: string;
    placeholder?: string;
    type?: HTMLInputTypeAttribute;
    step?: string | number;
}

export default function ControlledInputField({ name, control, rules, label, placeholder, type = 'text', step }: Props) {
    const isNumberInput = type === 'number';
    
    return (
        <ControlledCustomFormField name={name} control={control} rules={rules} label={label}>
            {({ field, fieldState: {error} }) => (
                <Input
                    type={type}
                    step={step ? step : undefined}
                    {...field}
                    onChange={(e) => {
                        const value = e.target.value;
                        if (isNumberInput && rules?.valueAsNumber) {
                            field.onChange(value === '' ? '' : Number(value));
                        } else {
                            field.onChange(value);
                        }
                    }}
                    placeholder={placeholder} 
                    style={{ borderColor: error ? 'red' : 'var(--sayso-border)' }}
                />
            )}
        </ControlledCustomFormField>
    )
}