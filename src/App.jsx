import { Routes, Route } from 'react-router-dom';

//VIEWS
import Login from './views/Login';
import Home from './views/Home';
import Account from './views/Account';

//COMPONENTS
import AuthGuard from './components/AuthGuard';
import GuestGuard from './components/GuestGuard';
import InsightPopUpWrapper from './components/InsightPopUpWrapper';
import Layout from './components/Layout';
import PermissionsModalContainer from './components/PermissionsModalContainer';
import TopDragBar from './components/TopDragBar';

//CONTEXT PROVIDERS
import { AuthProvider } from './context/AuthContext';
import { ProspectsProvider } from './context/ProspectsContext';
import { SalesCoachProvider } from './context/SalesCoachContext';
import { ToastProvider } from './context/ToastContext';

//STYLES
import './styles/App.css';
import './styles/Dashboard.css';

function App() {

  return (
      <ToastProvider>
        <AuthProvider>
          <ProspectsProvider>
              <SalesCoachProvider>
                <div className='App'>
                  {/* <TopDragBar /> */}
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
                  <PermissionsModalContainer />
                </div>
              </SalesCoachProvider>
          </ProspectsProvider>
        </AuthProvider>
      </ToastProvider>
  );
}

export default App; 
