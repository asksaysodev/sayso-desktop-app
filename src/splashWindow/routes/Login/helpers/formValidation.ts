import { LoginFormData } from "../types";

/**
 * Validates signup step 1 fields (personal info)
 */
export const validateStepOneFields = (formData: LoginFormData) => {
  const errors: Record<string, string> = {};
  if (!formData.name) errors.name = 'Name is required';
  if (!formData.lastname) errors.lastname = 'Last name is required';
  if (!formData.company) errors.company = 'Company is required';
  return errors;
};

/**
 * Validates login form fields
 */
export const validateLoginFields = (formData: LoginFormData) => {
  const errors: Record<string, string> = {};
  if (!formData.email) {
    errors.email = 'Email is required';
  } else if (!/\S+@\S+\.\S+/.test(formData.email)) {
    errors.email = 'Please enter a valid email';
  }
  if (!formData.password) errors.password = 'Password is required';
  return errors;
};