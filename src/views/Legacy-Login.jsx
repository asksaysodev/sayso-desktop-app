import React, { useState } from 'react';

import { useNavigate, useLocation } from 'react-router-dom';

import Loader from '../components/Loader';
import BtnSecondary from '../components/BtnSecondary';
import LoginBtn from '../components/LoginBtn';

import { useAuth } from '../context/AuthContext';

import logoVertical from '/assets/logo-pos-vertical.png';
import '../styles/Login.css';

const Login = () => {
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
  const location = useLocation();
  const { signIn, signUp } = useAuth();

  const handleInputChange = (e) => {
    const { name, value } = e.target;
    setFormData(prevState => ({
      ...prevState,
      [name]: value
    }));
    
    // Clear field error when user starts typing
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
    // Reset all fields
    setFormData({
      name: '',
      lastname: '',
      company: '',
      email: '',
      password: '',
      repeatPassword: ''
    });
  };

  const validateField = (name, value) => {
    switch (name) {
      case 'email':
        if (!value) return 'Email is required';
        if (!/\S+@\S+\.\S+/.test(value)) return 'Please enter a valid email';
        return '';
      case 'password':
        if (!value) return 'Password is required';
        if (value.length < 6) return 'Password must be at least 6 characters';
        return '';
      case 'repeatPassword':
        if (!value) return 'Please confirm your password';
        if (value !== formData.password) return 'Passwords do not match';
        return '';
      case 'name':
      case 'lastname':
      case 'company':
        if (!value) return 'This field is required';
        return '';
      default:
        return '';
    }
  };

  const handleNextStep = (e) => {
    e.preventDefault();
    setError(null);
    setFieldErrors({});
    
    // Validate first step fields
    const errors = {};
    if (!formData.name) errors.name = 'Name is required';
    if (!formData.lastname) errors.lastname = 'Last name is required';
    if (!formData.company) errors.company = 'Company is required';
    
    if (Object.keys(errors).length > 0) {
      setFieldErrors(errors);
      return;
    }
    setSignupStep(2);
  };

  const handleSubmit = async (e) => {
    e.preventDefault();
    setError(null);
    setFieldErrors({});

    // For signup step 1, just move to next step
    if (!isLoggingIn && signupStep === 1) {
      const errors = {};
      if (!formData.name) errors.name = 'Name is required';
      if (!formData.lastname) errors.lastname = 'Last name is required';
      if (!formData.company) errors.company = 'Company is required';
      
      if (Object.keys(errors).length > 0) {
        setFieldErrors(errors);
        return;
      }
      setSignupStep(2);
      return;
    }

    // Validate current form fields
    const errors = {};
    if (isLoggingIn) {
      if (!formData.email) errors.email = 'Email is required';
      else if (!/\S+@\S+\.\S+/.test(formData.email)) errors.email = 'Please enter a valid email';
      if (!formData.password) errors.password = 'Password is required';
    } else {
      if (!formData.email) errors.email = 'Email is required';
      else if (!/\S+@\S+\.\S+/.test(formData.email)) errors.email = 'Please enter a valid email';
      if (!formData.password) errors.password = 'Password is required';
      else if (formData.password.length < 6) errors.password = 'Password must be at least 6 characters';
      if (!formData.repeatPassword) errors.repeatPassword = 'Please confirm your password';
      else if (formData.password !== formData.repeatPassword) errors.repeatPassword = 'Passwords do not match';
    }

    if (Object.keys(errors).length > 0) {
      setFieldErrors(errors);
      return;
    }

    setIsBtnLoading(true);

    try {
      if (isLoggingIn) {
        const { error } = await signIn({
          email: formData.email,
          password: formData.password
        });
        if (error) throw error;
      } else {
        if (formData.password !== formData.repeatPassword) {
          throw new Error("Passwords don't match");
        }
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

      navigate('/', { replace: true });
    } catch (err) {
      setError(err.message);
      console.error('Authentication error:', err);
    } finally {
      setIsBtnLoading(false);
    }
  };

  if (isLoading) {
    return (
      <div className="loginContainer">
        <div className="loginForm">
          <Loader message={isLoggingIn ? "Signing in..." : "Creating account..."} />
        </div>
      </div>
    );
  }


  return (
    <div className="loginContainer">
      <div className="loginForm">
        <img src={logoVertical} alt="Sayso" style={{ display: 'block', margin: '20px auto', height: 100 }} />
        <h2>{isLoggingIn ? 'Welcome Back!' : `Create Account`}</h2>
        {error && <div className="errorMessage">{error}</div>}
        <form onSubmit={handleSubmit}>
          {isLoggingIn ? (
            <>
              <div className="formGroup">
                <label htmlFor="email">Email</label>
                <input
                  type="email"
                  id="email"
                  name="email"
                  className={`formInput ${fieldErrors.email ? 'error' : ''}`}
                  value={formData.email}
                  onChange={handleInputChange}
                />
                {fieldErrors.email && <div className="fieldError">{fieldErrors.email}</div>}
              </div>
              <div className="formGroup">
                <label htmlFor="password">Password</label>
                <input
                  type="password"
                  id="password"
                  name="password"
                  className={`formInput ${fieldErrors.password ? 'error' : ''}`}
                  value={formData.password}
                  onChange={handleInputChange}
                />
                {fieldErrors.password && <div className="fieldError">{fieldErrors.password}</div>}
              </div>
              <div className="formActions">
                <LoginBtn type="submit" text="Sign In" isLoading={isBtnLoading} isDisabled={isBtnLoading} />
              </div>
            </>
          ) : (
            signupStep === 1 ? (
              <>
                <div className="formGroup">
                  <label htmlFor="name">Name</label>
                  <input
                    type="text"
                    id="name"
                    name="name"
                    className={`formInput ${fieldErrors.name ? 'error' : ''}`}
                    value={formData.name}
                    onChange={handleInputChange}
                  />
                  {fieldErrors.name && <div className="fieldError">{fieldErrors.name}</div>}
                </div>
                <div className="formGroup">
                  <label htmlFor="lastname">Last Name</label>
                  <input
                    type="text"
                    id="lastname"
                    name="lastname"
                    className={`formInput ${fieldErrors.lastname ? 'error' : ''}`}
                    value={formData.lastname}
                    onChange={handleInputChange}
                  />
                  {fieldErrors.lastname && <div className="fieldError">{fieldErrors.lastname}</div>}
                </div>
                <div className="formGroup">
                  <label htmlFor="company">Company</label>
                  <input
                    type="text"
                    id="company"
                    name="company"
                    className={`formInput ${fieldErrors.company ? 'error' : ''}`}
                    value={formData.company}
                    onChange={handleInputChange}
                  />
                  {fieldErrors.company && <div className="fieldError">{fieldErrors.company}</div>}
                </div>
                <div className="formActions">
                  <LoginBtn type="submit" text="Continue" />
                </div>
              </>
            ) : (
              <>
                <div className="formGroup">
                  <label htmlFor="email">Email</label>
                  <input
                    type="email"
                    id="email"
                    name="email"
                    className={`formInput ${fieldErrors.email ? 'error' : ''}`}
                    value={formData.email}
                    onChange={handleInputChange}
                  />
                  {fieldErrors.email && <div className="fieldError">{fieldErrors.email}</div>}
                </div>
                <div className="formGroup">
                  <label htmlFor="password">Password</label>
                  <input
                    type="password"
                    id="password"
                    name="password"
                    className={`formInput ${fieldErrors.password ? 'error' : ''}`}
                    value={formData.password}
                    onChange={handleInputChange}
                  />
                  {fieldErrors.password && <div className="fieldError">{fieldErrors.password}</div>}
                </div>
                <div className="formGroup">
                  <label htmlFor="repeatPassword">Repeat Password</label>
                  <input
                    type="password"
                    id="repeatPassword"
                    name="repeatPassword"
                    className={`formInput ${fieldErrors.repeatPassword ? 'error' : ''}`}
                    value={formData.repeatPassword}
                    onChange={handleInputChange}
                  />
                  {fieldErrors.repeatPassword && <div className="fieldError">{fieldErrors.repeatPassword}</div>}
                </div>
                <div className="formActions">
                  <LoginBtn type="submit" text="Sign Up" isLoading={isBtnLoading} isDisabled={isBtnLoading} />
                  <div className='mt-20'>
                    <LoginBtn type="button" text="Back" onClick={() => setSignupStep(1)} isSecondary={true} />
                  </div>
                </div>
              </>
            )
          )}
        </form>
        <p className="toggleText" onClick={handleToggleMode}>
          {isLoggingIn ? "Don't have an account? Sign Up" : "Already have an account? Sign In"}
        </p>
      </div>
    </div>
  );
};

export default Login; 