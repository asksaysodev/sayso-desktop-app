import ReactDOM from 'react-dom/client';
import CoachSettingsWindowApp from './CoachSettingsWindowApp';
import './styles/CoachSettingsWindow.css';
import '@/styles/Colors.css';
import * as Sentry from "@sentry/electron/renderer";
import { sentryConfig } from '@/config/sentry';
import { QueryClient, QueryClientProvider } from '@tanstack/react-query';

Sentry.init(sentryConfig);

const queryClient = new QueryClient({
    defaultOptions: {
        queries: {
            staleTime: 1000 * 60 * 2,
            gcTime: 1000 * 60 * 5
        }
    }
});

const root = ReactDOM.createRoot(document.getElementById('coach-settings-window-root') as HTMLElement);
root.render(
    <QueryClientProvider client={queryClient}>
        <CoachSettingsWindowApp />
    </QueryClientProvider>
);
