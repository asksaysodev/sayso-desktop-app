import { useState } from 'react';

import { NavLink } from 'react-router-dom'
import {LuUsers, LuLogOut, LuChevronRight, LuSettings} from 'react-icons/lu'


import CoachCTA from './CoachCTA';
import Divider from './Divider';
import SaysoModal from './SaysoModal';

import { useAuth } from '../context/AuthContext';

import logoHorizontal from '/assets/logo-pos-horizontal.png';
import '../styles/Sidebar.css';


export default function Sidebar() {

    //STATE
    const [showSignOutModal, setShowSignOutModal] = useState(false);

    //HOOKS
    const { globalUser, handleSignOut } = useAuth();

    return (
        <div className="sidebar-container">
            {showSignOutModal && (
                <SaysoModal
                    title="Sign Out"
                    text="Are you sure you want to sign out?"
                    onConfirm={handleSignOut}
                    primaryText="Sign Out"
                    secondaryText="Cancel"
                    onDeny={() => setShowSignOutModal(false)}
                />
            )}
            <div className='full-w'>
                <div className="sidebar-header">
                    <img src={logoHorizontal} alt="Sayso Logo" />
                </div>
                <div className='sidebar-nav-container'>
                    <NavLink to="/" >
                        <div className="outline"></div>
                        <div className='sidebar-nav-item'>
                            <LuUsers />
                            <p>My Prospects</p>
                        </div>
                    </NavLink>
                    {globalUser?.isAdmin && (
                    <NavLink to="/admin" >
                        <div className="outline"></div>
                        <div className='sidebar-nav-item'>
                            <LuSettings />
                            <p>Admin</p>
                        </div>
                    </NavLink>
                    )}
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
                <div className='logout-button' onClick={() => setShowSignOutModal(true)}>
                    <LuLogOut />
                    <p>Log Out</p>
                </div>
            </div>
        </div>
    );
}