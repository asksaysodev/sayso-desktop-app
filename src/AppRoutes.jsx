import { Routes, Route, Navigate } from 'react-router-dom';

import AuthGuard from './components/AuthGuard';
import GuestGuard from './components/GuestGuard';
import Layout from './components/Layout';

import Login from './views/Login';
import PasswordRecovery from './views/PasswordRecovery';
import ResetPassword from './views/ResetPassword';
import Dashboard from './views/Dashboard';
import Account from './views/Account';
import StreamingTest from './components/StreamingTest';
import Checkout from './views/Checkout';
import Admin from './views/Admin';
import Subscription from './views/Subscription';

import { useAuth } from './context/AuthContext';
import { useMemo } from 'react';

export default function AppRoutes() {
    const { globalUser } = useAuth();
    const hasSubscription = useMemo(() => !!globalUser?.subscription_plan_id, [globalUser]);
    
    return (
        <Routes>
            <Route
                path="/login"
                element={
                    <GuestGuard>
                    <Login />
                    </GuestGuard>
                }
            />
            <Route
                path="/forgot-password"
                element={
                    <GuestGuard>
                    <PasswordRecovery />
                    </GuestGuard>
                }
            />
            <Route
                path="/reset-password"
                element={<ResetPassword />}
            />
            <Route
                path="/"
                element={
                    <AuthGuard>
                    <Layout>
                        {hasSubscription ? <Dashboard /> : <Navigate to="/subscription" replace />}
                    </Layout>
                    </AuthGuard>
                }
            />
            <Route
                path="/account"
                element={
                    <AuthGuard>
                    <Layout>
                        <Account />
                    </Layout>
                    </AuthGuard>
                }
            />
            <Route
                path="/streaming-test"
                element={
                    <AuthGuard>
                    <Layout>
                        <StreamingTest />
                    </Layout>
                    </AuthGuard>
                }
            />
            <Route
                path="/checkout"
                element={
                    <AuthGuard>
                    <Layout>
                        <Checkout />
                    </Layout>
                    </AuthGuard>
                }
            />
            <Route
                path="/admin"
                element={
                    <AuthGuard>
                    <Layout>
                        <Admin />
                    </Layout>
                    </AuthGuard>
                }
            />
            <Route
                path="/subscription"
                element={
                    <AuthGuard>
                    <Layout>
                        <Subscription />
                    </Layout>
                    </AuthGuard>
                }
            />
            <Route
                path="/checkout/success"
                element={
                    <AuthGuard>
                    <Layout>
                        <Checkout />
                    </Layout>
                    </AuthGuard>
                }
            />
        </Routes>
    )
}