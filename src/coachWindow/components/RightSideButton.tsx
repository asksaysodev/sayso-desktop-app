import { ReactNode } from 'react';

interface Props {
    onClick: () => void;
    children: ReactNode;
    disabled?: boolean;
    active?: boolean;
}

export default function RightSideButton({ onClick, children, disabled = false, active = false }: Props) {
    return (
        <button
            className={`right-side-coach-button ${active ? 'active' : ''}`}
            onClick={onClick}
            disabled={disabled}
        >
            {children}
        </button>
    );
}
