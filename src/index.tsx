import React from 'react';
import ReactDOM from 'react-dom/client';
import { HashRouter } from 'react-router-dom';
import App from './App';
import './index.css';
import './styles/App.css';
import './styles/Colors.css';
import * as Sentry from "@sentry/electron/renderer";
import { sentryConfig } from '@/config/sentry';

Sentry.init(sentryConfig);

ReactDOM.createRoot(document.getElementById('root') as HTMLElement).render(
  <React.StrictMode>
    <HashRouter>
      <App />
    </HashRouter>
  </React.StrictMode>
); 