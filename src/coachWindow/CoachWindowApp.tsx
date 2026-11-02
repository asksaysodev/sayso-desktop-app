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
const startsAsPrompt = new URLSearchParams(window.location.search).get('mode') === 'prompt';

function CoachWindowApp() {
  const [isPrompt, setIsPrompt] = useState(startsAsPrompt);
  // Once the prompt hands over, the coach pill grows out of the prompt's width
  const [wasPrompt, setWasPrompt] = useState(false);

  const handlePromoted = useCallback(() => {
    setWasPrompt(true);
    setIsPrompt(false);
  }, []);

  const handleLaunched = (leadType: LeadTypeEnum) => {
    const { setLeadType, cue_handleStartCue } = useCoachWindowStore.getState();
    setLeadType(leadType);
    cue_handleStartCue()
      .catch(reportCoachError)
      .finally(handlePromoted);
  };

  return (
      <QueryClientProvider client={queryClient}>
          {isPrompt
              ? <CoachPrompt onLaunched={handleLaunched} onPromoted={handlePromoted} />
              : <CoachWindowMain fromPrompt={wasPrompt} />}
      </QueryClientProvider>
  );
}

export default CoachWindowApp;
