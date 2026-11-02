import { useCallback, useState } from 'react';
import { QueryClient, QueryClientProvider } from '@tanstack/react-query';
import CoachWindowMain from './components/CoachWindowMain';
import CoachPrompt from './components/CoachPrompt';
import { useCoachWindowStore } from '../store/coachWindowStore';
import { reportCoachError } from '@/utils/errorReporting';
import type { LeadTypeEnum } from '@/types/coach';

const queryClient = new QueryClient({
    defaultOptions: {
        queries: {
            staleTime: 1000 * 60 * 2,
            gcTime: 1000 * 60 * 5
        }
    }
});

// SAYSO-566: main opens this window with ?mode=prompt when call detection spots
// a call. It turns into the coach when a lead is picked, or when the coach is
// opened from elsewhere (call-prompt:promoted). After a pick the session starts
// right away, but the prompt view stays (same look and width, "Starting
// conversation") until the start settles: then the coach view takes over and
// grows from the prompt's width, straight to active (or to idle with the
// error, if the start failed). No drag handle or settings button in between.
// While it starts, the coach view is already mounted but hidden, so its
// listeners (session expiry, auto-stop, …) are live for the whole start.
const startsAsPrompt = new URLSearchParams(window.location.search).get('mode') === 'prompt';

type View = 'prompt' | 'starting' | 'coach';

function CoachWindowApp() {
  const [view, setView] = useState<View>(startsAsPrompt ? 'prompt' : 'coach');
  // Once the prompt hands over, the coach pill grows out of the prompt's width
  const [wasPrompt, setWasPrompt] = useState(false);

  const handlePromoted = useCallback(() => {
    setWasPrompt(true);
    setView('coach');
  }, []);

  const handleLaunched = (leadType: LeadTypeEnum) => {
    const { setLeadType, cue_handleStartCue, isCoachLoading, isCoachActive } = useCoachWindowStore.getState();
    setWasPrompt(true);
    setView('starting');
    // Same double-start backstop as the coach's own Start buttons (SAYSO-355)
    if (isCoachLoading || isCoachActive) {
      handlePromoted();
      return;
    }
    setLeadType(leadType);
    cue_handleStartCue()
      .catch(reportCoachError)
      .finally(handlePromoted);
  };

  // Fixed slots, so the coach view mounted during 'starting' stays the same
  // instance when the prompt goes
  return (
      <QueryClientProvider client={queryClient}>
          {view !== 'coach' && <CoachPrompt onLaunched={handleLaunched} onPromoted={handlePromoted} />}
          {view !== 'prompt' && <CoachWindowMain fromPrompt={wasPrompt} hidden={view === 'starting'} />}
      </QueryClientProvider>
  );
}

export default CoachWindowApp;
