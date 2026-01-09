/**
 * Validates signup step 1 fields (personal info)
 */
export const validateStepOneFields = (formData) => {
  const errors = {};
  if (!formData.name) errors.name = 'Name is required';
  if (!formData.lastname) errors.lastname = 'Last name is required';
  if (!formData.company) errors.company = 'Company is required';
  return errors;
};

/**
 * Validates login form fields
 */
export const validateLoginFields = (formData) => {
  const errors = {};
  if (!formData.email) {
    errors.email = 'Email is required';
  } else if (!/\S+@\S+\.\S+/.test(formData.email)) {
    errors.email = 'Please enter a valid email';
  }
  if (!formData.password) errors.password = 'Password is required';
  return errors;
};

/**
 * Validates signup form fields (extends login validation)
 */
export const validateSignupFields = (formData) => {
  const errors = validateLoginFields(formData);
  
  if (!errors.password && formData.password.length < 6) {
    errors.password = 'Password must be at least 6 characters';
  }
  if (!formData.repeatPassword) {
    errors.repeatPassword = 'Please confirm your password';
  } else if (formData.password !== formData.repeatPassword) {
    errors.repeatPassword = 'Passwords do not match';
  }
  return errors;
};

/**
 * Validates password reset fields (password + confirmation only)
 */
export const validateResetPasswordFields = (formData) => {
  const errors = {};
  
  if (!formData.newPassword) {
    errors.newPassword = 'Password is required';
  } else if (formData.newPassword.length < 6) {
    errors.newPassword = 'Password must be at least 6 characters';
  }
  
  if (!formData.confirmPassword) {
    errors.confirmPassword = 'Please confirm your password';
  } else if (formData.newPassword !== formData.confirmPassword) {
    errors.confirmPassword = 'Passwords do not match';
  }
  
  return errors;
};

