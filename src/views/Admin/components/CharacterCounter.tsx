type ValidationStatus = 'short' | 'good' | 'long' | 'too-long';

function getValidationStatus(length: number): ValidationStatus {
    if (length < 150) return 'short';
    if (length < 320) return 'good';
    if (length < 380) return 'long';
    return 'too-long';
}

interface CharacterCounterProps {
    text: string;
}

export default function CharacterCounter({ text }: CharacterCounterProps) {
    const length = text.length;
    const status = getValidationStatus(length);
    
    const statusConfig = {
        'short': { label: 'Short', color: '#f59e0b' },
        'good': { label: 'Good', color: '#10b981' },
        'long': { label: 'Long', color: '#f97316' },
        'too-long': { label: 'Too Long', color: '#ef4444' }
    };
    
    const { label, color } = statusConfig[status];
    
    if (length === 0) return null;

    return (
        <div style={{ 
            display: 'flex', 
            fontSize: '12px',
            alignItems: 'center',
            gap: '8px',
            justifyContent: 'flex-end',
            color: color,
            fontWeight: 500
        }}>
            <span>{label}</span>
            <span>-</span>
            <span>{length} chars</span>
        </div>  
    );
}
