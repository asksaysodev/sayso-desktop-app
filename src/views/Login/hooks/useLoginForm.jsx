import { useAuth } from '@/context/AuthContext';
import { useState } from 'react';
import { useNavigate } from 'react-router-dom';
import { useForm } from 'react-hook-form';
import {
  validateStepOneFields,
  validateLoginFields,
  validateSignupFields
} from '../helpers/formValidation';
import * as Sentry from "@sentry/electron/renderer";

const INITIAL_VALUES = {
  name: '',
  lastname: '',
  company: '',
  email: '',
  password: '',
  repeatPassword: ''
};

export default function useLoginForm() {
  const [isLoggingIn, setIsLoggingIn] = useState(true);
  const [signupStep, setSignupStep] = useState(1);
  const [error, setError] = useState(null);
  const [isLoading, setIsLoading] = useState(false);
  const [isBtnLoading, setIsBtnLoading] = useState(false);
  const navigate = useNavigate();
  const { signIn, signUp } = useAuth();

  const customResolver = (values) => {
    let errors = {};

    if (!isLoggingIn && signupStep === 1) {
      errors = validateStepOneFields(values);
    } else if (isLoggingIn) {
      errors = validateLoginFields(values);
    } else {
      errors = validateSignupFields(values);
    }

    return {
      values: Object.keys(errors).length === 0 ? values : {},
      errors: Object.keys(errors).reduce((acc, key) => {
        acc[key] = { type: 'validation', message: errors[key] };
        return acc;
      }, {})
    };
  };

  const {
    control,
    reset,
    handleSubmit: rhfHandleSubmit,
    trigger
  } = useForm({
    resolver: customResolver,
    mode: 'onSubmit',
    reValidateMode: 'onBlur',
    defaultValues: INITIAL_VALUES
  });

  const handleToggleMode = () => {
    setIsLoggingIn(!isLoggingIn);
    setError(null);
    setSignupStep(1);
    reset(INITIAL_VALUES);
  };

  const handleNextStep = async (e) => {
    e.preventDefault();
    setError(null);

    const isValid = await trigger();
    if (isValid) {
      setSignupStep(2);
    }
  };

  const performAuthentication = async (data) => {
    if (isLoggingIn) {
      const { error } = await signIn({
        email: data.email,
        password: data.password
      });
      if (error) throw error;
    } else {
      const { error } = await signUp({
        email: data.email,
        password: data.password,
        options: {
          data: {
            name: data.name,
            lastname: data.lastname,
            company: data.company
          }
        }
      });
      if (error) throw error;
    }
  };

  const onSubmit = async (data) => {
    setError(null);

    if (!isLoggingIn && signupStep === 1) {
      setSignupStep(2);
      return;
    }

    setIsBtnLoading(true);

    try {
      await performAuthentication(data);
      navigate('/', { replace: true });
    } catch (err) {
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
    isLoggingIn,
    signupStep,
    error,
    isLoading,
    isBtnLoading,
    handleToggleMode,
    handleSubmit,
    setSignupStep
  }
}
