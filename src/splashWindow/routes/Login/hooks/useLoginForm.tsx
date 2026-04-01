import { useAuth } from '@/context/AuthContext';
import { useState } from 'react';
import { useNavigate } from 'react-router-dom';
import { useForm, FieldErrors } from 'react-hook-form';
import {
  validateLoginFields,
} from '../helpers/formValidation';
import * as Sentry from "@sentry/electron/renderer";
import { LoginFormData } from '../types';
import { getAAL } from '@/services/mfaServices';

const INITIAL_VALUES: LoginFormData = {
  email: '',
  password: '',
};

export default function useLoginForm() {
  const [error, setError] = useState<string | null>(null);
  const [isBtnLoading, setIsBtnLoading] = useState(false);
  const navigate = useNavigate();
  const { signIn, checkIfNeedsMFA } = useAuth();

  const customResolver = (values: LoginFormData) => {
    const errors = validateLoginFields(values);

    return {
      values: Object.keys(errors).length === 0 ? values : {},
      errors: Object.keys(errors).reduce<FieldErrors<LoginFormData>>((acc, key) => {
        acc[key as keyof LoginFormData] = { type: 'validation', message: errors[key as keyof typeof errors] };
        return acc;
      }, {})
    };
  };

  const {
    control,
    handleSubmit: rhfHandleSubmit,
  } = useForm<LoginFormData>({
    resolver: customResolver,
    mode: 'onSubmit',
    reValidateMode: 'onBlur',
    defaultValues: INITIAL_VALUES
  });

const performAuthentication = async (data: LoginFormData) => {
    const signInResult = await signIn({
        email: data.email,
        password: data.password
    });
    
    if (signInResult?.error) throw signInResult.error;
    
    let aalResult = await getAAL();
    
    if (aalResult.error) {
    aalResult = await getAAL();
    }
    
    if (aalResult.error || !aalResult.data) {                                                                                                                                                  
        navigate('/mfa-verify', { replace: true });                                                                                                                                              
        return;                                                                                                                                                                                  
    }       
    
    const needsMFA = checkIfNeedsMFA(aalResult.data.currentLevel, aalResult.data.nextLevel);
    
    if (needsMFA) {
        navigate('/mfa-verify', { replace: true });
    } else {
        navigate('/', { replace: true });
    }
  };

  const onSubmit = async (data: LoginFormData) => {
    setError(null);

    setIsBtnLoading(true);

    try {
      await performAuthentication(data);
    } catch (err: any) {
      setError(err.message);
      console.error('Authentication error:', err);
      Sentry.captureException(err);
    } finally {
      setIsBtnLoading(false);
    }
  };

  const handleSubmit = rhfHandleSubmit(onSubmit);

  return {
    control,
    error,
    isBtnLoading,
    handleSubmit,
  }
}
