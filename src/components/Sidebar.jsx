import { NavLink } from 'react-router-dom'
import {LuUsers, LuLogOut, LuChevronRight} from 'react-icons/lu'
import CoachCTA from './CoachCTA';
import '../styles/Sidebar.css';
import Divider from './Divider';

export default function Sidebar() {
  return (
    <div className="sidebar-container">
        <div className='full-w'>
            <div className="sidebar-header">
                {/* <img src='/assets/loader-logo.png' alt="Sayso Logo" /> */}
                <img src='/assets/sayso-logo.png' alt="Sayso Logo" />
                <h1>Sayso</h1>
            </div>
            <div className='sidebar-nav-container'>
                <NavLink to="/" >
                    <div className="outline"></div>
                    <div className='sidebar-nav-item'>
                        <LuUsers />
                        <p>My Prospects</p>
                    </div>
                </NavLink>
            </div>
        </div>
        <div className="sidebar-footer">
            <CoachCTA sidebar={true} active={false} />
            {/* <span className='sidebar-footer-divider'></span> */}
            <Divider />
            <NavLink to="/account" >
                <div className='account-widget'>
                    <div className='account-widget-active-container'>
                        <div className='account-widget-icon'>
                            <p>FS</p>
                        </div>
                    </div>
                    <div className='account-widget-info'>
                        <div className='account-widget-info-header'>
                            <h3>Frank Salsa</h3>
                            <LuChevronRight />
                        </div>
                        <p>frank@sayso.com</p>
                    </div>
                </div>
            </NavLink>
            {/* <span className='sidebar-footer-divider'></span> */}
            <Divider />
            <div className='logout-button'>
                <LuLogOut />
                <p>Log Out</p>
            </div>
        </div>
    </div>
  );
}