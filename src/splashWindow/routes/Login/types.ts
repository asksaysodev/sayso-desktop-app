// ===== React Hook Form types ===== //
export interface LoginFormData {
    email: string;
    password: string;
  }

  export interface ResetPasswordFormData {
    newPassword: string;
    confirmPassword: string;
  }

  export interface PasswordRecoveryFormData {
    email: string;
  }