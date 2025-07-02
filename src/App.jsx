import { Routes, Route, useLocation } from 'react-router-dom';
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


// Sales Checklist Component
const SalesChecklist = () => {
  return (
    <div className="salesChecklist">
      <h2>Sales Call Checklist</h2>
      <ul>
        <li>✅ Research prospect's company and role</li>
        <li>✅ Prepare relevant case studies</li>
        <li>✅ Set clear objectives for the call</li>
        <li>✅ Test your audio/video setup</li>
        <li>✅ Have your pitch deck ready</li>
        <li>✅ Prepare discovery questions</li>
        <li>✅ Set up next steps in advance</li>
      </ul>
    </div>
  );
};

function App() {
  const location = useLocation();

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