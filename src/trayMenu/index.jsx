import React from 'react';
import ReactDOM from 'react-dom/client';
import TrayMenuApp from './TrayMenuApp';
import './styles/TrayMenu.css';

const root = ReactDOM.createRoot(document.getElementById('tray-menu-root'));
root.render(
  <React.StrictMode>
    <TrayMenuApp />
  </React.StrictMode>
);

