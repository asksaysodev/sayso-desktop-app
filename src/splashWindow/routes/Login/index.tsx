import { useEffect, useState } from 'react';
import LoginInFormInputs from './components/LoginInFormInputs';
import useLoginForm from './hooks/useLoginForm';

import './styles.css';
import LoginLayout from '@/components/layouts/LoginLayout';

const SESSION_EXPIRED_NOTICE = 'Your session has expired. Please log in again.';

const Login = () => {
  const [notice, setNotice] = useState<string | null>(() => {
    const params = new URLSearchParams(window.location.search);
    const reason = params.get('reason');
    if (reason === 'session-expired') {
      history.replaceState(null, '', window.location.pathname);
      return SESSION_EXPIRED_NOTICE;
    }
    return null;
  });

  useEffect(() => {
    return window.electron?.ipcRenderer?.on('splash:show-reason', (reason: unknown) => {
      if (reason === 'session-expired') setNotice(SESSION_EXPIRED_NOTICE);
    });
  }, []);

  const handleSignUp = () => {
    window.electron?.openExternal('https://app.asksayso.com/login?signup=true');
  };

  const handleForgotPassword = () => {
    window.electron?.openExternal('https://app.asksayso.com/forgot-password');
  };

  const {
    control,
    error,
    isBtnLoading,
    isFormReady,
    handleSubmit,
  } = useLoginForm();

  return (
    <LoginLayout title={'Welcome Back!'} notice={notice} error={error}>
        <form onSubmit={handleSubmit}>
            <LoginInFormInputs
                control={control}
                isBtnLoading={isBtnLoading}
                isFormReady={isFormReady}
            />
        </form>
        <div className='toggle-text-container'>
            <p className="login-layout-toggle-text" onClick={handleSignUp}>
                Don't have an account? <span>Sign Up</span>
            </p>
            <p className="login-layout-toggle-text" onClick={handleForgotPassword}>
                Forgot password? <span>Click here</span>
            </p>
        </div>
    </LoginLayout>
  )
};

export default Login;
