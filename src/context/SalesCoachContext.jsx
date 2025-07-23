import { createContext, useContext, useState, useEffect, useRef, useCallback } from 'react';
import { getDynamicContext, runChatCompletion, trackSignals } from '../services/coachServices';

const SalesCoachContext = createContext();

export const SalesCoachProvider = ({ children }) => {

  //REFS
  const intervalRef = useRef(null);
  const timerStartedRef = useRef(false);
  const processTranscriptsRef = useRef(null);
  const transcriptionsRef = useRef([]);
  const checklistShownRef = useRef(false);
  
  //STATE
  const [isCallActive, setIsCallActive] = useState(false);
  const [isZoomInitialized, setIsZoomInitialized] = useState(false);
  const [transcriptions, setTranscriptions] = useState([]);
  const [insights, setInsights] = useState([]);
  const [currentInsight, setCurrentInsight] = useState(null);
  const [isCallStarting, setIsCallStarting] = useState(false);
  const [specificContext, setSpecificContext] = useState(null);
  const [prospectId, setProspectId] = useState(null);
  const [accountId, setAccountId] = useState(null);
  const [isChecklistVisible, setIsChecklistVisible] = useState(false);
  const [callProgress, setCallProgress] = useState(null);
  const [timerStartTime, setTimerStartTime] = useState(null);
  const [signalReceived, setSignalReceived] = useState(null);
  const [signals, setSignals] = useState([]);
  
  //FUNCTIONS
  const transformTranscriptionsToMessages = (transcriptions) => {
    return transcriptions.map(transcript => ({
      role: "user",
      content: `[${transcript.speaker.toUpperCase()}]: ${transcript.text}`
    }));
  };

  const calculateCallProgress = (elapsedMinutes) => {
    if (elapsedMinutes < 2) return "early";
    if (elapsedMinutes < 7) return "middle";
    return "late";
  };

  const updateCallProgress = () => {
    if (timerStartTime) {
      const elapsedMs = Date.now() - timerStartTime;
      const elapsedMinutes = elapsedMs / (1000 * 60);
      const progress = calculateCallProgress(elapsedMinutes);
      setCallProgress(progress);
    }
  };

  const handleGetDynamicContext = async ( recentConversation ) => {
    try {

      if( !prospectId || !accountId ) {
        throw new Error('Prospect ID and account ID are required');
      }

      const data = {
        text: recentConversation,
        prospectId: prospectId,
        accountId: accountId
      }

      const response = await getDynamicContext(data);
      
      // Extract the nested dynamicContext property
      const dynamicContext = response.dynamicContext;

      return dynamicContext;
      
    } catch (error) {
      console.error('❌ Error getting dynamic context:', error);
    }
  }

  const getLast45SecondsOfTranscripts = () => {
    const nowInSeconds = Math.floor(Date.now() / 1000);
    const cutoffTime = nowInSeconds - 45;
    
    const filtered = transcriptionsRef.current.filter(transcript => {
      let transcriptTime = transcript.timestamp;
      
      if (transcriptTime > 9999999999) {
        transcriptTime = Math.floor(transcriptTime / 1000);
      }
      
      const isRecent = transcriptTime >= cutoffTime;
      
      return isRecent;
    });
    
    return filtered;
  };

  const hasRecentTranscripts = () => {
    const recent = getLast45SecondsOfTranscripts();
    return recent.length > 0;
  };

  const processTranscriptsForInsights = useCallback(async () => {

    try {

      //WE NEED TO GET THE LAST 45 SECONDS OF THE CALL IN TRANSCRIPTIONS
      // Access transcriptions directly from the current state
      const currentTranscriptions = transcriptionsRef.current;
      
      const recentTranscripts = getLast45SecondsOfTranscripts();
      
      if (recentTranscripts.length === 0) {
        return;
      }
      
      //FORMAT THE TRANSCRIPTIONS INTO MESSAGES
      const transformedMessages = transformTranscriptionsToMessages(recentTranscripts);
      const conversationContext = transformedMessages.map(msg => msg.content).join('\n');
  
      //THEN WE NEED TO GET THE DYNAMIC CONTEXT
      const dynamicContext = await handleGetDynamicContext(conversationContext);

      const signals = await trackSignals(conversationContext);

      setSignalReceived(signals);

      const chatCompletionResponse = await runChatCompletion(conversationContext, dynamicContext, insights, signals); 

      if(chatCompletionResponse.Insight === 'yes' && chatCompletionResponse.Message !== '') {

        const message = {
          message: chatCompletionResponse.Message,
          isIceBreaker: false,
          insight: 'yes'
        };

        setInsights(prev => [...prev, message]);

        setCurrentInsight(message);
      }
      
    } catch (error) {
      console.error('❌ Error processing transcripts:', error);
    }
  }, [insights, prospectId, accountId]);

  // Store the latest version of the function in a ref
  processTranscriptsRef.current = processTranscriptsForInsights;

  // Transform signalReceived array into signals object, only updating from false to true
  useEffect(() => {
    if (!signalReceived || !Array.isArray(signalReceived)) {
      return;
    }

    setSignals(prevSignals => {
      const newSignals = { ...prevSignals };
      
      signalReceived.forEach(signalItem => {
        const { signal, detected } = signalItem;
        
        // Only update if the signal is detected (true)
        if (detected && (!(signal in prevSignals) || prevSignals[signal] === false)) {
          newSignals[signal] = true;
        } else if (!(signal in prevSignals)) {
          newSignals[signal] = false;
        }
      });
      
      return newSignals;
    });
  }, [signalReceived]);

  useEffect(() => {

    if (window.electron && window.electron.ipcRenderer) {
      const handleTranscriptionData = (data) => {
        if (data && typeof data === 'object' && 'text' in data) {

          const newTranscription = {
            text: data.text,
            speaker: data.speaker || 'unknown',
            timestamp: data.timestamp 
          };
          
          setTranscriptions(prev => {
            const updated = [...prev, newTranscription].sort((a, b) => a.timestamp - b.timestamp);
            // Update the ref with the latest transcriptions
            transcriptionsRef.current = updated;
            return updated;
          });
        }
      };

      const cleanup = window.electron.ipcRenderer.on('transcription-data', handleTranscriptionData);

      return () => {
        if (cleanup && typeof cleanup === 'function') {
          cleanup();
        }
      };
    }
  }, []);



  useEffect(() => {
    
    if (isCallActive && transcriptions.length > 2 && hasRecentTranscripts() && !timerStartedRef.current) {
      timerStartedRef.current = true;
      
      // 🎯 ONE-TIME TRIGGER GOES HERE - this will only run once when conditions are first met
      // Add your one-time logic here
      
      // Start your timer here
      const startTime = Date.now();
      
      // You can store the start time in state or ref if needed
      setTimerStartTime(startTime);
      
      // Add a small delay to ensure state is updated, then process
      setTimeout(() => {
        if (processTranscriptsRef.current) {
          processTranscriptsRef.current();
        }
      }, 1000);
      
      // Start the interval for subsequent processing
      intervalRef.current = setInterval(() => {
        if (processTranscriptsRef.current) {
          processTranscriptsRef.current();
        }
      }, 15000); // 15 seconds
      
    } else if (!isCallActive && timerStartedRef.current) {
      // Clean up when call ends
      clearInterval(intervalRef.current);
      intervalRef.current = null;
      timerStartedRef.current = false;
    }

    // Cleanup on unmount
    return () => {
      if (intervalRef.current) {
        clearInterval(intervalRef.current);
        timerStartedRef.current = false;
      }
    };
  }, [isCallActive, transcriptions.length > 2]); 
  
  useEffect(() => {
    if (isCallActive && transcriptions.length > 0 && !checklistShownRef.current) {
      setIsChecklistVisible(true);
      checklistShownRef.current = true;
    } else if (!isCallActive) {
      setIsChecklistVisible(false);
      checklistShownRef.current = false;
    }
  }, [isCallActive, transcriptions.length]);

  // Update call progress every minute when timer is active
  useEffect(() => {
    if (timerStartTime && isCallActive) {
      // Update immediately
      updateCallProgress();
      
      // Then update every minute
      const progressInterval = setInterval(updateCallProgress, 15000); // 15 seconds
      
      return () => clearInterval(progressInterval);
    } else if (!isCallActive) {
      setCallProgress(null);
      setTimerStartTime(null);
    }
  }, [timerStartTime, isCallActive]);

  

  const value = {
    isCallActive,
    setIsCallActive,
    isZoomInitialized,
    setIsZoomInitialized,
    currentInsight,
    setCurrentInsight,
    isCallStarting,
    setIsCallStarting,
    specificContext,
    setSpecificContext,
    setProspectId,
    setAccountId, 
    isChecklistVisible,
    setIsChecklistVisible,
    callProgress, 
    signals
  };

  return (
    <SalesCoachContext.Provider value={value}>
      {children}
    </SalesCoachContext.Provider>
  );
};

export const useSalesCoachContext = () => {
  const context = useContext(SalesCoachContext);
  if (!context) {
    throw new Error('useSalesCoachContext must be used within a SalesCoachProvider');
  }
  return context;
};
