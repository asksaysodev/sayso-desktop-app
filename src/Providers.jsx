//CONTEXT PROVIDERS
import { AuthProvider } from './context/AuthContext';
import { ProspectsProvider } from './context/ProspectsContext';
import { SalesCoachProvider } from './context/SalesCoachContext';
import { ToastProvider } from './context/ToastContext';

export default function Providers({ children }) {
    return (
        <ToastProvider>
            <AuthProvider>
                <ProspectsProvider>
                    <SalesCoachProvider>
                        {children}
                    </SalesCoachProvider>
                </ProspectsProvider>
            </AuthProvider>
        </ToastProvider>
    );
}