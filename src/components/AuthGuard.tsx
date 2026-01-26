import { Navigate, useLocation } from 'react-router-dom';

import SaysoLoader from './SaysoLoader';

import { useAuth } from '../context/AuthContext';

import '../styles/AuthGuard.css';
import { AuthContextValue } from '@/types/user';

interface Props {
  children: React.ReactNode;
}

const AuthGuard = ({ children }: Props) => {

  const { globalUser, loading = true, userLoading } = useAuth();
  const location = useLocation();

  // Show loading state while checking authentication
  if (loading || userLoading) {
    return <SaysoLoader />;
  }

  // If not authenticated, redirect to login
  if (!globalUser && !userLoading) {
    // Save the attempted URL to redirect back after login
    return <Navigate to="/login" state={{ from: location }} replace />;
  }

  // If authenticated, render the protected content
  return children;
};

export default AuthGuard; 