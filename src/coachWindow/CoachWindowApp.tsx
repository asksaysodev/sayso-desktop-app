import { QueryClient, QueryClientProvider } from '@tanstack/react-query';
import CoachWindowMain from './components/CoachWindowMain';

const queryClient = new QueryClient({
    defaultOptions: {
        queries: {
            staleTime: 1000 * 60 * 2,
            gcTime: 1000 * 60 * 5
        }
    }
});

function CoachWindowApp() {
  return (
      <QueryClientProvider client={queryClient}>
          <CoachWindowMain />
      </QueryClientProvider>
  );
}

export default CoachWindowApp;
