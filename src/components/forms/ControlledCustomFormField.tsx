import { Controller, ControllerFieldState, ControllerRenderProps, RegisterOptions, UseFormReturn, UseFormStateReturn } from "react-hook-form";
import { Label } from "../ui/label";

interface FieldOptions {
    field: ControllerRenderProps<any, string>;
    fieldState: ControllerFieldState;
    formState: UseFormStateReturn<any>;
}

interface Props {
    name: string;
    control: UseFormReturn<any>['control'];
    rules?: RegisterOptions;
    children: (fieldOptions: FieldOptions) => React.ReactNode;
    label?: string;
}

export default function ControlledCustomFormField({ name, control, rules, label, children }: Props) {
    return (
        <Controller
            name={name}
            control={control}
            rules={rules}
            render={(fieldOptions) => (
                <div>
                    <div className="grid gap-3">
                        {label && <Label htmlFor={name}>{label}</Label>}
                        {children(fieldOptions)}
                    </div>
                    {fieldOptions.fieldState.error && (
                        <span className="text-sm text-red-500">
                        {fieldOptions.fieldState.error?.message}
                        </span>
                    )}
                </div>
            )}
        />
    )
}