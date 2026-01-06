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
    control,
    isLoggingIn,
    signupStep,
    error,
    isLoading,
    isBtnLoading,
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
              control={control}
              isBtnLoading={isBtnLoading}
            />
          ) : (
            signupStep === 1 ? (
              <SignUpStepOneFormInputs
                control={control}
              />
            ) : (
                <SignUpStepTwoFormInputs
                  control={control}
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
