import React from 'react';
import ReactDOM from 'react-dom/client';
import CoachWindowApp from './CoachWindowApp';
import './styles/CoachWindow.css';
import './styles/Colors.css';
import * as Sentry from "@sentry/electron/renderer";
import { sentryConfig } from '@/config/sentry';

Sentry.init(sentryConfig);

const root = ReactDOM.createRoot(document.getElementById('coach-window-root'));
root.render(<CoachWindowApp />);
