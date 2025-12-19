import { RegisterOptions, UseFormReturn } from "react-hook-form";
import { Textarea } from "../ui/textarea";
import ControlledCustomFormField from "./ControlledCustomFormField";
import CharacterCounter from "@/views/Admin/components/CharacterCounter";

interface Props {
    name: string;
    control: UseFormReturn<any>['control'];
    rules?: RegisterOptions;
    label?: string;
    placeholder?: string;
    rows?: number;
    resize?: 'none' | 'vertical' | 'horizontal';
    characterCounter?: boolean;
}

export default function ControlledTextareaField({ 
    name, 
    control, 
    rules, 
    label, 
    placeholder, 
    rows = 2, 
    resize = 'vertical', 
    characterCounter = false 
}: Props) {

    return (
        <ControlledCustomFormField name={name} control={control} rules={rules} label={label}>
            {({ field, fieldState: {error} }) => (
                <div className="grid gap-1">
                    <Textarea
                        {...field}
                        rows={rows}
                        placeholder={placeholder} 
                        style={{ 
                            borderColor: error ? 'red' : 'var(--sayso-border)',
                            resize: resize,
                            outline: 'none'
                        }}
                    />
                    {characterCounter && <CharacterCounter text={field.value || ''} />}
                </div>
            )}
        </ControlledCustomFormField>
    )
}