import React from 'react';
import './styles.css';

/**
 * Reusable form input component with label and error handling.
 * 
 * @param {string} type - Input type (text, email, password, etc.)
 * @param {string} id - Input ID for label association
 * @param {string} name - Input name attribute
 * @param {string} label - Label text to display above input
 * @param {string} value - Controlled input value
 * @param {function} onChange - Change handler function
 * @param {string} error - Error message to display (if any)
 * @param {string} className - Additional CSS classes for the input
 * @param {object} rest - Any additional props to pass to the input element
 */
const CustomInput = ({ 
  type = 'text', 
  id, 
  name, 
  label, 
  value, 
  onChange, 
  error, 
  className = '',
  ...rest 
}) => {
  return (
    <div className="formGroup">
      <label htmlFor={id}>{label}</label>
      <input
        type={type}
        id={id}
        name={name}
        className={`formInput ${error ? 'error' : ''} ${className}`}
        value={value}
        onChange={onChange}
        {...rest}
      />
      {error && <div className="fieldError">{error}</div>}
    </div>
  );
};

export default CustomInput;

