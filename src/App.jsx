import { Routes, Route, useLocation, useNavigate } from 'react-router-dom';
import './styles/App.css';
import Dashboard from './views/Dashboard';
import Login from './views/Login';
import AuthGuard from './components/AuthGuard';
import { AuthProvider } from './context/AuthContext';
import { ProspectsProvider } from './context/ProspectsContext';
import ZoomCallback from './views/ZoomCallback';
import GuestGuard from './components/GuestGuard';
import { SalesCoachProvider } from './context/SalesCoachContext';
import InsightPopUpWrapper from './components/InsightPopUpWrapper';
import { ZoomClient } from './views/ZoomClient';
import PostCall from './views/PostCall';
import FloatingChecklistContainer from './components/FloatingChecklistContainer';
import { useEffect } from 'react';

function App() {
  const location = useLocation();
  const navigate = useNavigate();

  // Add debug logging for location changes
  useEffect(() => {
    console.log('📍 App: Location changed to:', location.pathname, location.hash);
  }, [location]);

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
      console.log('🔌 Setting up reset-to-home listener...');
      
      const cleanup = window.electron.ipcRenderer.on('reset-to-home', (params) => {
        console.log('🎯 Received reset-to-home from main process!', params);
        console.log('🔍 DEBUG: params type:', typeof params);
        console.log('🔍 DEBUG: params keys:', params ? Object.keys(params) : 'null/undefined');
        
        // Extract meetingId and prospectId from params
        const { meetingId, prospectId } = params || {};
        console.log('🔍 DEBUG: Extracted meetingId:', meetingId, 'prospectId:', prospectId);
        
        if (meetingId && prospectId) {
          const targetRoute = `/post-call/${meetingId}/${prospectId}`;
          console.log('🚀 Navigating to:', targetRoute);
          // Use React Router's navigate instead of window.location.hash
          navigate(targetRoute);
          console.log('✅ Navigation completed');
        } else {
          console.log('⚠️ No meetingId or prospectId found, navigating to home');
          // Use React Router's navigate instead of window.location.hash
          navigate('/');
        }
      });
      
      return () => {
        console.log('🧹 Cleaning up reset-to-home listener...');
        if (cleanup && typeof cleanup === 'function') {
          cleanup();
        }
      };
    }
  }, [navigate]);

  return (
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
                        <Dashboard />
                      </AuthGuard>
                    }
                  />
                  <Route path="/zoom-callback" element={<ZoomCallback />} />
                  <Route path="/post-call/:meetingId/:prospectId" element={<PostCall />} />
                  <Route path="/zoom-client-new/:meetingId/:prospectId" element={<ZoomClient />} />
                </Routes>
                <InsightPopUpWrapper />
              </div>
              <FloatingChecklistContainer />
            </SalesCoachProvider>
        </ProspectsProvider>
      </AuthProvider>
  );
}

export default App; 