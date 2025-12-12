import { useAuth } from '@/context/AuthContext';
import { useState } from 'react';
import { useNavigate } from 'react-router-dom';
import { 
  validateStepOneFields, 
  validateLoginFields, 
  validateSignupFields 
} from '../helpers/formValidation';

export default function useLoginForm() {
  const [isLoggingIn, setIsLoggingIn] = useState(true);
  const [signupStep, setSignupStep] = useState(1);
  const [error, setError] = useState(null);
  const [isLoading, setIsLoading] = useState(false);
  const [formData, setFormData] = useState({
    name: '',
    lastname: '',
    company: '',
    email: '',
    password: '',
    repeatPassword: ''
  });
  const [fieldErrors, setFieldErrors] = useState({});
  const [isBtnLoading, setIsBtnLoading] = useState(false);
  const navigate = useNavigate();
  const { signIn, signUp } = useAuth();

  const handleInputChange = (e) => {
    const { name, value } = e.target;
    setFormData(prevState => ({
      ...prevState,
      [name]: value
    }));
    
    if (fieldErrors[name]) {
      setFieldErrors(prev => ({
        ...prev,
        [name]: ''
      }));
    }
  };

  const handleToggleMode = () => {
    setIsLoggingIn(!isLoggingIn);
    setError(null);
    setFieldErrors({});
    setSignupStep(1);
    setFormData({
      name: '',
      lastname: '',
      company: '',
      email: '',
      password: '',
      repeatPassword: ''
    });
  };

  const handleNextStep = (e) => {
    e.preventDefault();
    setError(null);
    setFieldErrors({});
    
    const errors = validateStepOneFields(formData);
    if (Object.keys(errors).length > 0) {
      setFieldErrors(errors);
      return;
    }
    setSignupStep(2);
  };

  const performAuthentication = async () => {
    if (isLoggingIn) {
      const { error } = await signIn({
        email: formData.email,
        password: formData.password
      });
      if (error) throw error;
    } else {
      const { error } = await signUp({
        email: formData.email,
        password: formData.password,
        options: {
          data: {
            name: formData.name,
            lastname: formData.lastname,
            company: formData.company
          }
        }
      });
      if (error) throw error;
    }
  };

  const handleSubmit = async (e) => {
    e.preventDefault();
    setError(null);
    setFieldErrors({});

    if (!isLoggingIn && signupStep === 1) {
      const errors = validateStepOneFields(formData);
      if (Object.keys(errors).length > 0) {
        setFieldErrors(errors);
        return;
      }
      setSignupStep(2);
      return;
    }

    const errors = isLoggingIn 
      ? validateLoginFields(formData)
      : validateSignupFields(formData);
    
    if (Object.keys(errors).length > 0) {
      setFieldErrors(errors);
      return;
    }

    setIsBtnLoading(true);

    try {
      await performAuthentication();
      navigate('/', { replace: true });
    } catch (err) {
      setError(err.message);
      console.error('Authentication error:', err);
    } finally {
      setIsBtnLoading(false);
    }
  };

  return {
    isLoggingIn,
    signupStep,
    error,
    isLoading,
    formData,
    fieldErrors,
    isBtnLoading,
    handleInputChange,
    handleToggleMode,
    handleSubmit,
    setSignupStep
  }
}