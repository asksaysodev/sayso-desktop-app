import ReactDOM from 'react-dom/client';
import CoachSettingsWindowApp from './CoachSettingsWindowApp';
import './styles/CoachSettingsWindow.css';
import '@/styles/Colors.css';
import * as Sentry from "@sentry/electron/renderer";
import { sentryConfig } from '@/config/sentry';

Sentry.init(sentryConfig);

const root = ReactDOM.createRoot(document.getElementById('coach-settings-window-root') as HTMLElement);
root.render(<CoachSettingsWindowApp />);
