import LoginLoader from './components/LoginLoader';
import LoginInFormInputs from './components/LoginInFormInputs';
import useLoginForm from './hooks/useLoginForm';

import './styles.css';
import LoginLayout from '@/components/layouts/LoginLayout';

const Login = () => {
  const handleSignUp = () => {
    window.electron?.openExternal('https://app.asksayso.com/login?signup=true');
  };

  const handleForgotPassword = () => {
    window.electron?.openExternal('https://app.asksayso.com/forgot-password');
  };

  const {
    control,
    error,
    isLoading,
    isBtnLoading,
    handleSubmit,
  } = useLoginForm();

  if (isLoading) {
    return <LoginLoader />
  }

  return (
    <LoginLayout title={'Welcome Back!'} error={error}>
        <form onSubmit={handleSubmit}>
            <LoginInFormInputs
                control={control}
                isBtnLoading={isBtnLoading}
            />
        </form>
        <p className="toggleText" onClick={handleSignUp}>
            {"Don't have an account? Sign Up"}
        </p>
        <p className="toggleText forgotPasswordText" onClick={handleForgotPassword}>
            Forgot password? Click here
        </p>
    </LoginLayout>
  )
};

export default Login;
