import { useEffect } from 'react';
import { Routes, Route, useLocation, useNavigate } from 'react-router-dom';
import Dashboard from './views/Dashboard';
import Login from './views/Login';
import Home from './views/Home';
import AuthGuard from './components/AuthGuard';
import { AuthProvider } from './context/AuthContext';
import { ProspectsProvider } from './context/ProspectsContext';
import GuestGuard from './components/GuestGuard';
import { SalesCoachProvider } from './context/SalesCoachContext';
import InsightPopUpWrapper from './components/InsightPopUpWrapper';
import FloatingChecklistContainer from './components/FloatingChecklistContainer';
import './styles/App.css';
import { ToastProvider } from './context/ToastContext';
import Account from './views/Account';
import Layout from './components/Layout';

function App() {

  const location = useLocation();
  const navigate = useNavigate();
  
  // Conditionally apply styles based on route
  const isFloatingWindow = location.pathname === '/' || location.pathname === '/sales-checklist';
  const isDashboard = location.pathname === '/dashboard';

  // Determine root class based on route
  let rootClassName = '';
  if (isFloatingWindow) {
    rootClassName = 'App'; // Class for floating windows
  } else if (isDashboard) {
    rootClassName = 'DashboardApp'; // Class for dashboard
  }

  useEffect(() => {
    // Check if we're in Electron and the API is available
    if (window.electron && window.electron.ipcRenderer) {
      const cleanup = window.electron.ipcRenderer.on('reset-to-home', (params) => {
        // Extract meetingId and prospectId from params
        const { meetingId, prospectId, sessionId } = params || {};
        
        if (meetingId && prospectId) {
          const targetRoute = `/post-call/${meetingId}/${prospectId}/${sessionId}`; 
          // Use React Router's navigate instead of window.location.hash
          navigate(targetRoute);
        } else {
          // Use React Router's navigate instead of window.location.hash
          navigate('/');
        }
      });
      
      return () => {  
        if (cleanup && typeof cleanup === 'function') {
          cleanup();
        }
      };
    }
  }, [navigate]);

  return (
      <ToastProvider>
        <AuthProvider>
          <ProspectsProvider>
              <SalesCoachProvider>
                <div className={rootClassName}>
                <Routes>
                  <Route path="/login" element={<GuestGuard><Login /></GuestGuard>} />
                  <Route
                    path="/"
                    element={
                      <AuthGuard>
                        <Layout>
                          <Home />
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

                </Routes>
                <InsightPopUpWrapper />
                </div>
                <FloatingChecklistContainer />
              </SalesCoachProvider>
          </ProspectsProvider>
        </AuthProvider>
      </ToastProvider>
  );
}

export default App; 