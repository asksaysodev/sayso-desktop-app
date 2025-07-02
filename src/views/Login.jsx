import React, { useState } from 'react';
import { useNavigate, useLocation } from 'react-router-dom';
import { useAuth } from '../context/AuthContext';
import Loader from '../components/Loader';
import '../styles/Login.css';
import BtnPrimary from '../components/BtnPrimary';
import BtnSecondary from '../components/BtnSecondary';

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
  };

  const handleToggleMode = () => {
    setIsLoggingIn(!isLoggingIn);
    setError(null);
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

  const handleNextStep = (e) => {
    e.preventDefault();
    setError(null);
    // Validate first step fields
    if (!formData.name || !formData.lastname || !formData.company) {
      setError('Please fill in all fields');
      return;
    }
    setSignupStep(2);
  };

  const handleSubmit = async (e) => {
    e.preventDefault();
    setError(null);

    // For signup step 1, just move to next step
    if (!isLoggingIn && signupStep === 1) {
      if (!formData.name || !formData.lastname || !formData.company) {
        setError('Please fill in all fields');
        return;
      }
      setSignupStep(2);
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

      // Redirect to the page they tried to visit or dashboard
      const from = location.state?.from?.pathname || '/';
      navigate(from, { replace: true });
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

  const renderSignupStep1 = () => (
    <>
      <div className="formGroup">
        <label htmlFor="name">Name</label>
        <input
          type="text"
          id="name"
          name="name"
          className="formInput"
          value={formData.name}
          onChange={handleInputChange}
          required
        />
      </div>
      <div className="formGroup">
        <label htmlFor="lastname">Last Name</label>
        <input
          type="text"
          id="lastname"
          name="lastname"
          className="formInput"
          value={formData.lastname}
          onChange={handleInputChange}
          required
        />
      </div>
      <div className="formGroup">
        <label htmlFor="company">Company</label>
        <input
          type="text"
          id="company"
          name="company"
          className="formInput"
          value={formData.company}
          onChange={handleInputChange}
          required
        />
      </div>
      <div className="formActions">
        <BtnPrimary type="submit" text="Continue" />
      </div>
    </>
  );

  const renderSignupStep2 = () => (
    <>
      <div className="formGroup">
        <label htmlFor="email">Email</label>
        <input
          type="email"
          id="email"
          name="email"
          className="formInput"
          value={formData.email}
          onChange={handleInputChange}
          required
        />
      </div>
      <div className="formGroup">
        <label htmlFor="password">Password</label>
        <input
          type="password"
          id="password"
          name="password"
          className="formInput"
          value={formData.password}
          onChange={handleInputChange}
          required
        />
      </div>
      <div className="formGroup">
        <label htmlFor="repeatPassword">Repeat Password</label>
        <input
          type="password"
          id="repeatPassword"
          name="repeatPassword"
          className="formInput"
          value={formData.repeatPassword}
          onChange={handleInputChange}
          required
        />
      </div>
      <div className="formActions">
        <BtnPrimary type="submit" text="Sign Up" isLoading={isBtnLoading} isDisabled={isBtnLoading} />
        <div className='mt-20'>
          <BtnSecondary type="button" text="Back" onClick={() => setSignupStep(1)} />
        </div>
      </div>
    </>
  );

  return (
    <div className="loginContainer">
      <div className="loginForm">
        <img src="/assets/logo.png" alt="Logo" style={{ display: 'block', margin: '20px auto', width: 72, height: 72 }} />
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
                  className="formInput"
                  value={formData.email}
                  onChange={handleInputChange}
                  required
                />
              </div>
              <div className="formGroup">
                <label htmlFor="password">Password</label>
                <input
                  type="password"
                  id="password"
                  name="password"
                  className="formInput"
                  value={formData.password}
                  onChange={handleInputChange}
                  required
                />
              </div>
              <div className="formActions">
                <BtnPrimary type="submit" text="Sign In" isLoading={isBtnLoading} isDisabled={isBtnLoading} />
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
                    className="formInput"
                    value={formData.name}
                    onChange={handleInputChange}
                    required
                  />
                </div>
                <div className="formGroup">
                  <label htmlFor="lastname">Last Name</label>
                  <input
                    type="text"
                    id="lastname"
                    name="lastname"
                    className="formInput"
                    value={formData.lastname}
                    onChange={handleInputChange}
                    required
                  />
                </div>
                <div className="formGroup">
                  <label htmlFor="company">Company</label>
                  <input
                    type="text"
                    id="company"
                    name="company"
                    className="formInput"
                    value={formData.company}
                    onChange={handleInputChange}
                    required
                  />
                </div>
                <div className="formActions">
                  <BtnPrimary type="submit" text="Continue" />
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
                    className="formInput"
                    value={formData.email}
                    onChange={handleInputChange}
                    required
                  />
                </div>
                <div className="formGroup">
                  <label htmlFor="password">Password</label>
                  <input
                    type="password"
                    id="password"
                    name="password"
                    className="formInput"
                    value={formData.password}
                    onChange={handleInputChange}
                    required
                  />
                </div>
                <div className="formGroup">
                  <label htmlFor="repeatPassword">Repeat Password</label>
                  <input
                    type="password"
                    id="repeatPassword"
                    name="repeatPassword"
                    className="formInput"
                    value={formData.repeatPassword}
                    onChange={handleInputChange}
                    required
                  />
                </div>
                <div className="formActions">
                  <BtnPrimary type="submit" text="Sign Up" isLoading={isBtnLoading} isDisabled={isBtnLoading} />
                  <div className='mt-20'>
                    <BtnSecondary type="button" text="Back" onClick={() => setSignupStep(1)} />
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