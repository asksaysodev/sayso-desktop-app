import { ReactNode } from 'react';

interface Props {
    onClick: () => void;
    children: ReactNode;
    disabled?: boolean;
}

export default function RightSideButton({ onClick, children, disabled = false }: Props) {
    return (
        <button className='right-side-coach-button' onClick={onClick} disabled={disabled}>
            {children}
        </button>
    );
}
