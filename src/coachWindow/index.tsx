import ReactDOM from 'react-dom/client';
import CoachWindowApp from './CoachWindowApp';
import './styles/CoachWindow.css';
import './styles/Colors.css';
import * as Sentry from "@sentry/electron/renderer";
import { sentryConfig } from '@/config/sentry';
import '@/utils/platform';
import '@/services/networkReporter';

Sentry.init(sentryConfig);

// Apply font size before first render to avoid a visible jump
const initialFontSize = new URLSearchParams(window.location.search).get('fontSize') ?? 's';
if (initialFontSize !== 's') {
    document.documentElement.dataset.fontSize = initialFontSize;
}

// Keep in sync with live changes from coach settings window
window.electron?.ipcRenderer?.on('font-size-changed', (size) => {
    const s = size as string;
    if (s === 's') {
        delete document.documentElement.dataset.fontSize;
    } else {
        document.documentElement.dataset.fontSize = s;
    }
});

const root = ReactDOM.createRoot(document.getElementById('coach-window-root') as HTMLElement);
root.render(<CoachWindowApp />);
