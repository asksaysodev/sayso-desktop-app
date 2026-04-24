import ReactDOM from 'react-dom/client';
import PlaybookWindowApp from './PlaybookWindowApp';
import '@/coachWindow/styles/Colors.css';
import './styles/PlaybookWindow.css';
import * as Sentry from "@sentry/electron/renderer";
import { sentryConfig } from '@/config/sentry';

Sentry.init(sentryConfig);

const initialFontSize = new URLSearchParams(window.location.search).get('fontSize') ?? 's';
if (initialFontSize !== 's') {
    document.documentElement.dataset.fontSize = initialFontSize;
}

window.electron?.ipcRenderer?.on('font-size-changed', (size) => {
    const s = size as string;
    if (s === 's') {
        delete document.documentElement.dataset.fontSize;
    } else {
        document.documentElement.dataset.fontSize = s;
    }
});

const root = ReactDOM.createRoot(document.getElementById('playbook-window-root') as HTMLElement);
root.render(<PlaybookWindowApp />);
