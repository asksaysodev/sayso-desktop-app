//CONTEXT PROVIDERS
import { AuthProvider } from './context/AuthContext';
import { ProspectsProvider } from './context/ProspectsContext';
import { SalesCoachProvider } from './context/SalesCoachContext';
import { ToastProvider } from './context/ToastContext';
import { CoachWindowProvider } from './context/CoachWindowContext';

export default function Providers({ children }) {
    return (
        <ToastProvider>
            <AuthProvider>
                <ProspectsProvider>
                    <SalesCoachProvider>
                        <CoachWindowProvider>
                            {children}
                        </CoachWindowProvider>
                    </SalesCoachProvider>
                </ProspectsProvider>
            </AuthProvider>
        </ToastProvider>
    );
}