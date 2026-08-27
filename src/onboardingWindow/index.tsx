import React from 'react';
import ReactDOM from 'react-dom/client';
import OnboardingWindowApp from './OnboardingWindowApp';
import '@/utils/platform';
import '@/services/networkReporter';

ReactDOM.createRoot(document.getElementById('onboarding-window-root')!).render(
    <React.StrictMode>
        <OnboardingWindowApp />
    </React.StrictMode>
);
