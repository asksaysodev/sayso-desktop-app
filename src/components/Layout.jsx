import Sidebar from './Sidebar';
import TopDragBar from './TopDragBar';

import '../styles/Home.css';

export default function Layout({ children }) {
  return (
    // <div className="layout-container">
    //   <TopDragBar />
    //   <div className="home-container">
    //     <Sidebar />
    //     {children}
    //   </div>
    // </div>
    <div className="home-container">
      <TopDragBar />
      <Sidebar />
      {children}
    </div>
  );
}
