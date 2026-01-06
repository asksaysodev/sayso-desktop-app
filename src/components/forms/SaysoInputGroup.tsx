import { InputGroupAddon, InputGroupInput } from "../ui/input-group";

import { InputGroup } from "../ui/input-group";

interface Props {
    placeholder: string;
    value: string;
    onChange: (e: React.ChangeEvent<HTMLInputElement>) => void;
    icon: React.ReactNode;
    size?: number;
    className?: string;
}

export default function SaysoInputGroup({ placeholder, value, onChange, icon, size, className }: Props) {
    return (
        <InputGroup className={className}>
            <InputGroupInput
                size={size}
                placeholder={placeholder}
                value={value}
                onChange={onChange}
            />
            <InputGroupAddon>
                {icon && icon}
            </InputGroupAddon>
        </InputGroup>
    );
}