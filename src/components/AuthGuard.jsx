import React from 'react';
import { Navigate, useLocation } from 'react-router-dom';
import { useAuth } from '../context/AuthContext';
import Loader from './Loader';
import '../styles/AuthGuard.css';

const AuthGuard = ({ children }) => {
  const { globalUser, loading = true, userLoading } = useAuth();
  const location = useLocation();

  // console.log('[AuthGuard] location.pathname:', location.pathname);
  // console.log('[AuthGuard] loading:', loading, '| userLoading:', userLoading, '| globalUser:', globalUser);

  // Skip auth check for /zoom-success
  if (location.pathname === '/zoom-success') {
    console.log('[AuthGuard] Skipping auth check for /zoom-success');
    return children;
  }

  // Show loading state while checking authentication
  if (loading || userLoading) {
    // console.log('[AuthGuard] Showing loader (loading or userLoading is true)');
    return <Loader />;
  }

  // If not authenticated, redirect to login
  if (!globalUser && !userLoading) {
    // console.log('[AuthGuard] Not authenticated, redirecting to /login');
    // Save the attempted URL to redirect back after login
    return <Navigate to="/login" state={{ from: location }} replace />;
  }

  // If authenticated, render the protected content
  // console.log('[AuthGuard] Authenticated, rendering children');
  return children;
};

export default AuthGuard; 