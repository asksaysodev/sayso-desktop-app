//CONTEXT PROVIDERS
import { AuthProvider } from './context/AuthContext';
import { ProspectsProvider } from './context/ProspectsContext';
import { SalesCoachProvider } from './context/SalesCoachContext';
import { SubscriptionAlertProvider } from './context/SubscriptionAlertContext';
import { ToastProvider } from './context/ToastContext';
import { QueryClient, QueryClientProvider } from '@tanstack/react-query';

interface Props {
    children: React.ReactNode;
}

const queryClient = new QueryClient();

export default function Providers({ children }: Props) {
    return (
      <QueryClientProvider client={queryClient}>
          <ToastProvider>
              <AuthProvider>
                  <ProspectsProvider>
                    <SubscriptionAlertProvider>
                        <SalesCoachProvider>
                            {children}
                        </SalesCoachProvider>
                    </SubscriptionAlertProvider>
                  </ProspectsProvider>
              </AuthProvider>
          </ToastProvider>
      </QueryClientProvider>
    );
}
