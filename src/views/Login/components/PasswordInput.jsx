import { useState } from 'react';
import { LuEye, LuEyeOff } from 'react-icons/lu';
import './PasswordInput.css';

export default function PasswordInput({
  id,
  name,
  label,
  value,
  onChange,
  onBlur,
  error,
  className = '',
  ...rest
}) {
    const [showPassword, setShowPassword] = useState(false);

    const togglePasswordVisibility = () => {
        setShowPassword(prev => !prev);
    };

    return (
        <div className="customInputFormGroup">
        <label htmlFor={id}>{label}</label>
        <div className="passwordInputWrapper">
            <input
            type={showPassword ? 'text' : 'password'}
            id={id}
            name={name}
            className={`formInput ${error ? 'error' : ''} ${className}`}
            value={value}
            onChange={onChange}
            onBlur={onBlur}
            {...rest}
            />
            <button
            type="button"
            className="passwordToggleButton"
            onClick={togglePasswordVisibility}
            aria-label={showPassword ? 'Hide password' : 'Show password'}
            >
            {showPassword ? <LuEyeOff size={18} /> : <LuEye size={18} />}
            </button>
        </div>
        {error && <div className="fieldError">{error}</div>}
        </div>
    );
}

