import { LoginFormData } from "../types";

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