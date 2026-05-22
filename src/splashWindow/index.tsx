import ReactDOM from 'react-dom/client';
import { HashRouter } from 'react-router-dom';
import '../index.css';
import './styles/SplashWindowApp.css';
import './styles/Colors.css';
import * as Sentry from "@sentry/electron/renderer";
import { sentryConfig } from '@/config/sentry';
import '@/services/networkReporter';
import SplashWindowApp from './SplashWindowApp';

Sentry.init(sentryConfig);

const root = ReactDOM.createRoot(document.getElementById('splash-window-root') as HTMLElement);
root.render(
    <HashRouter>
        <SplashWindowApp />
    </HashRouter>
);
