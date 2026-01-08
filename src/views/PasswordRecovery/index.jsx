import React from 'react';
import { useNavigate } from 'react-router-dom';

import './styles.css';
import LoginLayout from '@/components/layouts/LoginLayout';
import { useForm } from 'react-hook-form';
import ControlledInputField from '@/components/forms/ControlledInputField';
import { supabase } from '@/config/supabase';
import SaysoButton from '@/components/SaysoButton';
import { useMutation } from '@tanstack/react-query';
import { useToast } from '@/context/ToastContext';

const PasswordRecovery = () => {
  const navigate = useNavigate();
  const { showToast } = useToast();
  const { control, handleSubmit, setError, reset } = useForm({
    defaultValues: {
      email: ''
    }
  });
  
  const {mutate: resetPasswordMutation, isPending: isResetPasswordPending} = useMutation({
    mutationFn: async (email) => {
      const { data, error } = await supabase.auth.resetPasswordForEmail(email);
      if (error) throw error;
      return data;
    },
    onSuccess: () => {
      showToast('success', 'Reset link sent! Check your email');
      reset();
    },
    onError: (error) => {
      setError('email', { 
        message: error.message || 'Failed to send reset link, try again later or contact support.' 
      });
    }
  });

  const onSubmit = async (data) => {
    if (!data.email) {
      setError('email', { message: 'Email is required' });
      return;
    }
    resetPasswordMutation(data.email);
  };

  const handleGoBackToLogin = () => {
    if (isResetPasswordPending) return;
    navigate('/login');
  };

  return (
    <LoginLayout title="Reset Your Password">
        <p className='password-recovery-description'>
          Enter your email address and we'll send you a link to reset your password.
        </p>
        <form onSubmit={handleSubmit(onSubmit)}>
          <div style={{ marginBottom: '20px' }}>
              <ControlledInputField type="email" name="email" label="Email Address" control={control} labelCn='password-recovery-input-label'/>
          </div>
          <SaysoButton 
            label="Send Reset Link" 
            type="submit" 
            onClick={handleSubmit(onSubmit)} 
            loading={isResetPasswordPending}
            fullWidth
          />
        </form>
        <p 
          className="toggleText"
          onClick={handleGoBackToLogin}
        >
          Back to Sign In
        </p>
      </LoginLayout>
  );
};

export default PasswordRecovery;

