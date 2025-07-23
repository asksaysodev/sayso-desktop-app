import React from 'react';
import { Navigate, useLocation } from 'react-router-dom';
import { useAuth } from '../context/AuthContext';
import Loader from './Loader';
import '../styles/AuthGuard.css';

const AuthGuard = ({ children }) => {
  const { globalUser, loading = true, userLoading } = useAuth();
  const location = useLocation();

  // Skip auth check for /zoom-success
  if (location.pathname === '/zoom-success') {
    return children;
  }

  // Show loading state while checking authentication
  if (loading || userLoading) {
    return <Loader />;
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