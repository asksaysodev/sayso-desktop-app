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
  const [isCoachWindowOpen, setIsCoachWindowOpen] = useState(false);

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
  const [sessionId, setSessionId] = useState(null);


  
  //FUNCTIONS
  // const openCoachWindow = () => {
    
  //   if (window.electron && window.electron.ipcRenderer) {
  //     window.electron.ipcRenderer.send('open-coach-window');
  //     setIsCoachWindowOpen(true);
  //   } else {
  //     console.warn('Electron not available, cannot open coach window');
  //     setIsCoachWindowOpen(false);
  //   }
  // }

  // const closeCoachWindow = () => {
    
  //   if (window.electron && window.electron.ipcRenderer) {
  //     window.electron.ipcRenderer.send('close-coach-window');
  //     setIsCoachWindowOpen(false);
  //   }
  // }

  // Listen for coach window close events from the coach window itself
  useEffect(() => {
    if (window.electron && window.electron.ipcRenderer) {
      const handleCoachWindowClosed = () => {
        setIsCoachWindowOpen(false);
      };

      window.electron.ipcRenderer.on('coach-window-closed', handleCoachWindowClosed);

      return () => {
        window.electron.ipcRenderer.removeAllListeners('coach-window-closed');
      };
    }
  }, []);

  // const transformTranscriptionsToMessages = (transcriptions) => {
  //   return transcriptions.map(transcript => ({
  //     role: "user",
  //     content: `[${transcript.speaker.toUpperCase()}]: ${transcript.text}`
  //   }));
  // };

  // const calculateCallProgress = (elapsedMinutes) => {
  //   if (elapsedMinutes < 2) return "early";
  //   if (elapsedMinutes < 7) return "middle";
  //   return "late";
  // };

  // const updateCallProgress = () => {
  //   if (timerStartTime) {
  //     const elapsedMs = Date.now() - timerStartTime;
  //     const elapsedMinutes = elapsedMs / (1000 * 60);
  //     const progress = calculateCallProgress(elapsedMinutes);
  //     setCallProgress(progress);
  //   }
  // };

  // const handleGetDynamicContext = async ( recentConversation ) => {
  //   try {

  //     if( !prospectId || !accountId ) {
  //       throw new Error('Prospect ID and account ID are required');
  //     }

  //     const data = {
  //       text: recentConversation,
  //       prospectId: prospectId,
  //       accountId: accountId
  //     }

  //     const response = await getDynamicContext(data);
      
  //     // Extract the nested dynamicContext property
  //     const dynamicContext = response.dynamicContext;

  //     return dynamicContext;
      
  //   } catch (error) {
  //     console.error('❌ Error getting dynamic context:', error);
  //   }
  // }

  // const getLast45SecondsOfTranscripts = () => {
  //   const nowInSeconds = Math.floor(Date.now() / 1000);
  //   const cutoffTime = nowInSeconds - 45;
    
  //   console.log(' [getLast45SecondsOfTranscripts] Current time:', nowInSeconds, 'Cutoff:', cutoffTime);
  //   console.log('🔍 [getLast45SecondsOfTranscripts] Total transcripts in ref:', transcriptionsRef.current.length);
    
  //   const filtered = transcriptionsRef.current.filter(transcript => {
  //     let transcriptTime = transcript.timestamp;
      
  //     if (transcriptTime > 9999999999) {
  //       transcriptTime = Math.floor(transcriptTime / 1000);
  //     }
      
  //     const isRecent = transcriptTime >= cutoffTime;
      
  //     console.log('🔍 [getLast45SecondsOfTranscripts] Transcript time:', transcriptTime, 'isRecent:', isRecent, 'text:', transcript.text?.substring(0, 50));
      
  //     return isRecent;
  //   });
    
  //   console.log('🔍 [getLast45SecondsOfTranscripts] Filtered recent transcripts:', filtered.length);
  //   return filtered;
  // };

  // const hasRecentTranscripts = () => {
  //   const recent = getLast45SecondsOfTranscripts();
  //   const hasRecent = recent.length > 0;
  //   console.log('🔍 [hasRecentTranscripts] Has recent transcripts:', hasRecent, 'count:', recent.length);
  //   return hasRecent;
  // };

  // const processTranscriptsForInsights = useCallback(async () => {
  //   console.log('🚀 [processTranscriptsForInsights] ===== STARTING INSIGHT PROCESSING =====');
  //   console.log('🔍 [processTranscriptsForInsights] Current state:', {
  //     isCallActive,
  //     transcriptionsCount: transcriptions.length,
  //     prospectId,
  //     accountId,
  //     insightsCount: insights.length
  //   });

  //   try {
  //     //WE NEED TO GET THE LAST 45 SECONDS OF THE CALL IN TRANSCRIPTIONS
  //     const recentTranscripts = getLast45SecondsOfTranscripts();
  //     console.log('🔍 [processTranscriptsForInsights] Recent transcripts count:', recentTranscripts.length);
      
  //     if (recentTranscripts.length === 0) {
  //       console.log('⚠️ [processTranscriptsForInsights] No recent transcripts, skipping insight processing');
  //       return;
  //     }
      
  //     //FORMAT THE TRANSCRIPTIONS INTO MESSAGES
  //     const transformedMessages = transformTranscriptionsToMessages(recentTranscripts);
  //     const conversationContext = transformedMessages.map(msg => msg.content).join('\n');
  //     console.log('🔍 [processTranscriptsForInsights] Conversation context length:', conversationContext.length);
  //     console.log('🔍 [processTranscriptsForInsights] Conversation preview:', conversationContext.substring(0, 200) + '...');
  
  //     //GET THE DYNAMIC CONTEXT
  //     console.log('🔍 [processTranscriptsForInsights] Getting dynamic context...');
  //     const dynamicContext = await handleGetDynamicContext(conversationContext);
  //     console.log(' [processTranscriptsForInsights] Dynamic context retrieved:', dynamicContext);

  //     console.log('🔍 [processTranscriptsForInsights] Tracking signals...');
  //     const signals = await trackSignals(conversationContext);
  //     console.log('🔍 [processTranscriptsForInsights] Signals tracked:', signals);

  //     setSignalReceived(signals);

  //     console.log('🔍 [processTranscriptsForInsights] Running chat completion...');
  //     const chatCompletionResponse = await runChatCompletion(conversationContext, dynamicContext, insights, signals); 
  //     console.log('🔍 [processTranscriptsForInsights] Chat completion response:', chatCompletionResponse);

  //     if(chatCompletionResponse.Insight === 'yes' && chatCompletionResponse.Message !== '') {
  //       console.log(' [processTranscriptsForInsights] ===== NEW INSIGHT GENERATED! =====');
  //       console.log('🎯 [processTranscriptsForInsights] Message:', chatCompletionResponse.Message);

  //       const message = {
  //         message: chatCompletionResponse.Message,
  //         isIceBreaker: false,
  //         insight: 'yes'
  //       };

  //       setInsights(prev => [...prev, message]);
  //       setCurrentInsight(message);
  //       console.log('🎯 [processTranscriptsForInsights] Insight added to state');
  //     } else {
  //       console.log('🔍 [processTranscriptsForInsights] No insight generated - Insight:', chatCompletionResponse.Insight, 'Message length:', chatCompletionResponse.Message?.length || 0);
  //     }
      
  //   } catch (error) {
  //     console.error('❌ [processTranscriptsForInsights] Error processing transcripts:', error);
  //   }
    
  //   console.log('🏁 [processTranscriptsForInsights] ===== INSIGHT PROCESSING COMPLETE =====');
  // }, [insights, prospectId, accountId]);

  // // Store the latest version of the function in a ref
  // processTranscriptsRef.current = processTranscriptsForInsights;

  // Transform signalReceived array into signals object, only updating from false to true
  // useEffect(() => {
  //   if (!signalReceived || !Array.isArray(signalReceived)) {
  //     return;
  //   }

  //   setSignals(prevSignals => {
  //     const newSignals = { ...prevSignals };
      
  //     signalReceived.forEach(signalItem => {
  //       const { signal, detected } = signalItem;
        
  //       // Only update if the signal is detected (true)
  //       if (detected && (!(signal in prevSignals) || prevSignals[signal] === false)) {
  //         newSignals[signal] = true;
  //       } else if (!(signal in prevSignals)) {
  //         newSignals[signal] = false;
  //       }
  //     });
      
  //     return newSignals;
  //   });
  // }, [signalReceived]);

  // useEffect(() => {
  //   console.log('🔍 [useEffect] Setting up transcription listener');

  //   if (window.electron && window.electron.ipcRenderer) {
  //     const handleTranscriptionData = (data) => {
  //       console.log('🎤 [handleTranscriptionData] Received transcription data:', data);
        
  //       if (data && typeof data === 'object' && 'text' in data) {
  //         const newTranscription = {
  //           text: data.text,
  //           speaker: data.speaker || 'unknown',
  //           timestamp: data.timestamp 
  //         };
          
  //         console.log(' [handleTranscriptionData] Processing new transcription:', {
  //           text: newTranscription.text,
  //           speaker: newTranscription.speaker,
  //           timestamp: newTranscription.timestamp
  //         });
          
  //         setTranscriptions(prev => {
  //           const updated = [...prev, newTranscription].sort((a, b) => a.timestamp - b.timestamp);
  //           // Update the ref with the latest transcriptions
  //           transcriptionsRef.current = updated;
  //           console.log('🎤 [handleTranscriptionData] Updated transcriptions count:', updated.length);
  //           console.log('🎤 [handleTranscriptionData] Updated transcriptions ref count:', transcriptionsRef.current.length);
  //           return updated;
  //         });
  //       } else {
  //         console.warn('🎤 [handleTranscriptionData] Invalid transcription data received:', data);
  //       }
  //     };

  //     const cleanup = window.electron.ipcRenderer.on('transcription-data', handleTranscriptionData);

  //     return () => {
  //       if (cleanup && typeof cleanup === 'function') {
  //         cleanup();
  //       }
  //     };
  //   } else {
  //     console.warn('🎤 [useEffect] Electron not available for transcription listener');
  //   }
  // }, []);



  // useEffect(() => {
  //   console.log('🔍 [useEffect] Insight processing effect triggered:', {
      
  //     transcriptionsLength: transcriptions.length,
  //     hasRecentTranscripts: hasRecentTranscripts(),
  //     timerStarted: timerStartedRef.current
  //   });
    
  //   if (transcriptions.length > 2 && hasRecentTranscripts() && !timerStartedRef.current) {
  //     console.log('🚀 [useEffect] ===== STARTING INSIGHT TIMER =====');
  //     console.log('🔍 [useEffect] Conditions met - starting insight processing timer');

  //     timerStartedRef.current = true;
      
  //     const startTime = Date.now();
      
  //     // You can store the start time in state or ref if needed
  //     setTimerStartTime(startTime);
      
  //     // Add a small delay to ensure state is updated, then process
  //     setTimeout(() => {
  //       console.log('🔍 [useEffect] Initial timeout triggered - calling processTranscriptsForInsights');
  //       if (processTranscriptsRef.current) {
  //         processTranscriptsRef.current();
  //       } else {
  //         console.error('❌ [useEffect] processTranscriptsRef.current is null!');
  //       }
  //     }, 1000);
      
  //     // Start the interval for subsequent processing
  //     intervalRef.current = setInterval(() => {
  //       console.log('🔍 [useEffect] Interval triggered - calling processTranscriptsForInsights');
  //       if (processTranscriptsRef.current) {
  //         processTranscriptsRef.current();
  //       } else {
  //         console.error('❌ [useEffect] processTranscriptsRef.current is null!');
  //       }
  //     }, 15000); // 15 seconds
      
  //     console.log('🔍 [useEffect] Timer started with 15-second interval');
      
  //   } else if (!isCallActive && timerStartedRef.current) {
  //     console.log('🛑 [useEffect] ===== STOPPING INSIGHT TIMER =====');
  //     console.log('🔍 [useEffect] Call ended - cleaning up timer');

  //     // Clean up when call ends
  //     clearInterval(intervalRef.current);
  //     intervalRef.current = null;
  //     timerStartedRef.current = false;
  //   } else {
  //     console.log(' [useEffect] Conditions not met for starting timer:', {
  //       isCallActive,
  //       transcriptionsLength: transcriptions.length,
  //       hasRecentTranscripts: hasRecentTranscripts(),
  //       timerStarted: timerStartedRef.current
  //     });
  //   }

  //   // Cleanup on unmount
  //   return () => {
  //     if (intervalRef.current) {
  //       console.log('🧹 [useEffect] Cleanup - clearing interval');
  //       clearInterval(intervalRef.current);
  //       timerStartedRef.current = false;
  //     }
  //   };
  // }, [isCallActive, transcriptions.length]); 
  
  // useEffect(() => {
  //   if (isCallActive && transcriptions.length > 0 && !checklistShownRef.current) {
  //     setIsChecklistVisible(true);
  //     checklistShownRef.current = true;
  //   } else if (!isCallActive) {
  //     setIsChecklistVisible(false);
  //     checklistShownRef.current = false;
  //   }
  // }, [isCallActive, transcriptions.length]);

  // Update call progress every minute when timer is active
  // useEffect(() => {
  //   if (timerStartTime && isCallActive) {
  //     // Update immediately
  //     updateCallProgress();
      
  //     // Then update every minute
  //     const progressInterval = setInterval(updateCallProgress, 15000); // 15 seconds
      
  //     return () => clearInterval(progressInterval);
  //   } else if (!isCallActive) {
  //     setCallProgress(null);
  //     setTimerStartTime(null);
  //   }
  // }, [timerStartTime, isCallActive]);

  

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
    signals,
    sessionId,
    setSessionId,

    // openCoachWindow,
    // closeCoachWindow,
    // isCoachWindowOpen
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
