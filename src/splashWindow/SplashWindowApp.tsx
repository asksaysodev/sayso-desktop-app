import { QueryClient, QueryClientProvider } from '@tanstack/react-query';
import { AuthProvider } from '@/context/AuthContext';
import LogoutGate from './LogoutGate';
import UpdateGate from './UpdateGate';
import SplashWindowRoutes from './SplashWindowRoutes';

const queryClient = new QueryClient();

export default function SplashWindowApp() {
    return (
        <QueryClientProvider client={queryClient}>
            <LogoutGate>
                <AuthProvider>
                    <UpdateGate>
                        <SplashWindowRoutes />
                    </UpdateGate>
                </AuthProvider>
            </LogoutGate>
        </QueryClientProvider>
    );
}
