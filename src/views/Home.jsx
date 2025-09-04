import Sidebar from '../components/Sidebar';
import ProspectsContainer from '../components/ProspectsContainer';
import '../styles/Home.css';

export default function Home() {

  return (
    <div className="home-container">
        <Sidebar />
        <ProspectsContainer />
    </div>
  );
}