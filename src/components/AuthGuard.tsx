import { useEffect, useState } from 'react';
import { Navigate, useLocation, useNavigate } from 'react-router-dom';

import SaysoLoader from './SaysoLoader';

import { useAuth } from '../context/AuthContext';

import '../styles/AuthGuard.css';

interface Props {
  children: React.ReactNode;
}

const AuthGuard = ({ children }: Props) => {
  const { globalUser, loading = true, userLoading, mfaRequired } = useAuth();
  const location = useLocation();
  const navigate = useNavigate();
  const [isCheckingMFA, setIsCheckingMFA] = useState(true);

  useEffect(() => {
    const checkMFAStatus = () => {
      if (!globalUser) {
        setIsCheckingMFA(false);
        return;
      }

      if (mfaRequired) {
        navigate('/mfa-verify', { replace: true });
      } else {
        setIsCheckingMFA(false);
      }
    };

    if (!loading && !userLoading) {
      checkMFAStatus();
    }
  }, [globalUser, loading, userLoading, mfaRequired, navigate]);

  if (loading || userLoading || isCheckingMFA) {
    return <SaysoLoader />;
  }

  if (!globalUser) {
    return <Navigate to="/login" state={{ from: location }} replace />;
  }

  return children;
};

export default AuthGuard; 