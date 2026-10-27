import React from 'react';
import ReactDOM from 'react-dom/client';
import CallDetectedApp from './CallDetectedApp';
import './styles/CallDetected.css';
import * as Sentry from "@sentry/electron/renderer";
import { sentryConfig } from '@/config/sentry';
import '@/utils/platform';
import '@/services/networkReporter';

Sentry.init(sentryConfig);

const root = ReactDOM.createRoot(document.getElementById('call-detected-root') as HTMLElement);
root.render(
  <React.StrictMode>
    <CallDetectedApp />
  </React.StrictMode>
);
