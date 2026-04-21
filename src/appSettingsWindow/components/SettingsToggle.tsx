type Option<T extends string> = { value: T; label: string };

type Props<T extends string> = {
    options: Option<T>[];
    value: T;
    onChange: (value: T) => void;
};

export default function SettingsToggle<T extends string>({ options, value, onChange }: Props<T>) {
    return (
        <div className="cue-mode-toggle">
            {options.map(option => (
                <button
                    key={option.value}
                    className={`cue-mode-option ${value === option.value ? 'active' : ''}`}
                    onClick={() => onChange(option.value)}
                >
                    {option.label}
                </button>
            ))}
        </div>
    );
}
