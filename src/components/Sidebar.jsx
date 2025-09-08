import { NavLink } from 'react-router-dom'
import {LuUsers, LuLogOut, LuChevronRight} from 'react-icons/lu'

import CoachCTA from './CoachCTA';
import Divider from './Divider';

import { useAuth } from '../context/AuthContext';

import '../styles/Sidebar.css';

export default function Sidebar() {

  //HOOKS
  const { globalUser } = useAuth();

//   //FUNCTIONS
//   const handleLogout = () => {
//     signOut();
//   }

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
            <Divider />
            <NavLink to="/account" >
                <div className='account-widget'>
                    <div className='account-widget-active-container'>
                        <div className='account-widget-icon'>
                            <p>{globalUser?.name?.charAt(0)}{globalUser?.lastname?.charAt(0)}</p>
                        </div>
                    </div>
                    <div className='account-widget-info'>
                        <div className='account-widget-info-header'>
                            <h3>{globalUser?.name} {globalUser?.lastname}</h3>
                            <LuChevronRight />
                        </div>
                        <p>{globalUser?.email}</p>
                    </div>
                </div>
            </NavLink>
            <Divider />
            <div className='logout-button'>
                <LuLogOut />
                <p>Log Out</p>
            </div>
        </div>
    </div>
  );
}