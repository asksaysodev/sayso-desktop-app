import React from 'react';
import ReactDOM from 'react-dom/client';
import OnboardingWindowApp from './OnboardingWindowApp';
import * as Sentry from "@sentry/electron/renderer";
import { sentryConfig } from '@/config/sentry';
import '@/utils/platform';
import '@/services/networkReporter';

Sentry.init(sentryConfig);

ReactDOM.createRoot(document.getElementById('onboarding-window-root')!).render(
    <React.StrictMode>
        <OnboardingWindowApp />
    </React.StrictMode>
);
