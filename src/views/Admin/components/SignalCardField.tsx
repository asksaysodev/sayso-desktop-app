import { PropsWithChildren } from "react";

interface Props extends PropsWithChildren {
    fieldName: string;
}

export default function SignalCardField({ fieldName, children }: Props) {
    return (
        <div className='signal-card-field'>
            <label htmlFor={fieldName}>{fieldName}</label>
            {children}
        </div>
    )
}