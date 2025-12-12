import React from 'react';

import logoVertical from '/assets/logo-pos-vertical.png';
import LoginLoader from './components/LoginLoader';
import LoginInFormInputs from './components/LoginInFormInputs';
import SignUpStepOneFormInputs from './components/SignUpStepOneFormInputs';
import SignUpStepTwoFormInputs from './components/SignUpStepTwoFormInputs';
import useLoginForm from './hooks/useLoginForm';

import './styles.css';

const Login = () => {
  const { 
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
  } = useLoginForm();

  if (isLoading) {
    return <LoginLoader isLoggingIn={isLoggingIn}/>
  }

  return (
    <div className="loginContainer">
      <div className="loginForm">
        <img src={logoVertical} alt="Sayso" style={{ display: 'block', margin: '20px auto', height: 100 }} />
        <h2>{isLoggingIn ? 'Welcome Back!' : `Create Account`}</h2>
        {error && <div className="errorMessage">{error}</div>}
        <form onSubmit={handleSubmit}>
          {isLoggingIn ? (
            <LoginInFormInputs 
              formData={formData}
              handleInputChange={handleInputChange}
              fieldErrors={fieldErrors}
              isBtnLoading={isBtnLoading}
            />
          ) : (
            signupStep === 1 ? (
              <SignUpStepOneFormInputs
                formData={formData}
                handleInputChange={handleInputChange}
                fieldErrors={fieldErrors}
              />
            ) : (
                <SignUpStepTwoFormInputs
                  formData={formData}
                  handleInputChange={handleInputChange}
                  fieldErrors={fieldErrors}
                  isBtnLoading={isBtnLoading}
                  setSignupStep={setSignupStep}
                />
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