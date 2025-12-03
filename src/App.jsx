import { Routes, Route } from 'react-router-dom';

//VIEWS
import Login from './views/Login';
import Home from './views/Home';
import Account from './views/Account';
import Admin  from './views/Admin/Admin';

//COMPONENTS
import AuthGuard from './components/AuthGuard';
import GuestGuard from './components/GuestGuard';
import InsightPopUpWrapper from './components/InsightPopUpWrapper';
import Layout from './components/Layout';
import PermissionsModalContainer from './components/PermissionsModalContainer';
import StreamingTest from './components/StreamingTest';

//STYLES
import './styles/App.css';
import './styles/Dashboard.css';
import Providers from './Providers';

function App() {
  
  return (
    <Providers>
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
            path="/admin"
            element={
              <AuthGuard>
                <Layout>
                  <Admin />
                </Layout>
              </AuthGuard>
            }
          />
        </Routes>
        <InsightPopUpWrapper />
        <PermissionsModalContainer />
      </div>
    </Providers>
  );
}

export default App; 
