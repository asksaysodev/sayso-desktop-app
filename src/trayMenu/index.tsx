import React from 'react';
import ReactDOM from 'react-dom/client';
import TrayMenuApp from './TrayMenuApp';
import './styles/TrayMenu.css';
import * as Sentry from "@sentry/electron/renderer";
import { sentryConfig } from '@/config/sentry';

Sentry.init(sentryConfig);

const root = ReactDOM.createRoot(document.getElementById('tray-menu-root') as HTMLElement);
root.render(
  <React.StrictMode>
    <TrayMenuApp />
  </React.StrictMode>
);

