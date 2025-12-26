//COMPONENTS
import InsightPopUpWrapper from './components/InsightPopUpWrapper';
import PermissionsModalContainer from './components/PermissionsModalContainer';

//STYLES
import './styles/App.css';
import './styles/Dashboard.css';
import Providers from './Providers';
import AppRoutes from './AppRoutes';

function App() {

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