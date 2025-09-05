import Sidebar from './Sidebar';
import '../styles/Home.css';

export default function Layout({ children }) {
  return (
    <div className="home-container">
      <Sidebar />
      {children}
    </div>
  );
}
