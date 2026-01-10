//COMPONENTS
import InsightPopUpWrapper from './components/InsightPopUpWrapper';
import PermissionsModalContainer from './components/PermissionsModalContainer';

//STYLES
import './styles/App.css';
import './styles/Dashboard.css';
import Providers from './Providers';
import AppRoutes from './AppRoutes';
import { useEffect } from 'react';
import { useNavigate, useLocation } from 'react-router-dom';

function App() {
  const navigate = useNavigate();
  const location = useLocation();

  // Intercept password recovery tokens in URL hash to redirect to reset-password instead of login the user in automatically
  useEffect(() => {
    const hash = window.location.hash;

    if (hash.includes('type=recovery') && hash.includes('access_token=')) {
      console.log('[App] Password recovery detected in URL, redirecting to reset-password');

      const hashContent = hash.split('#')[1];
      if (hashContent && hashContent.includes('access_token')) {
        const [route, params] = hashContent.split('?');

        if (!route.includes('/reset-password')) {
          navigate(`/reset-password?${params || hashContent}`, { replace: true });
        }
      }
    }
  }, [navigate, location]);

  return (
    <Providers>
      <div className='App'>
        <AppRoutes />
        <InsightPopUpWrapper />
        <PermissionsModalContainer />
      </div>
    </Providers>
  );
}

export default App;
