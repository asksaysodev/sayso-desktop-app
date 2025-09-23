import { createContext, useContext, useState, useEffect, useRef, useCallback } from 'react';
import { runCoach } from '../services/coachServices';
import { v4 } from 'uuid';

import useCoach from '../coachWindow/hooks/useCoach';
import useAudio from '../coachWindow/hooks/useAudio';
import useInsightScheduler from '../coachWindow/hooks/useInsightScheduler';

const CoachWindowContext = createContext();

export const CoachWindowProvider = ({ children }) => {

    //STATE
    const [isCoachWindowOpen, setIsCoachWindowOpen] = useState(false)
    const [prospects, setProspects] = useState([])
    const [prospectId, setProspectId] = useState(null)
    const [sessionId, setSessionId] = useState(null)
    const [iceBreaker, setIceBreaker] = useState(null)
    const [globalUser, setGlobalUser] = useState(null)
    const [isCoachActive, setIsCoachActive] = useState(false)
    const [isCoachLoading, setIsCoachLoading] = useState(false)
    const [callDurationInSeconds, setCallDurationInSeconds] = useState(0)
    const [transcriptions, setTranscriptions] = useState([]);
    const [receivedInsights, setReceivedInsights] = useState([]);
    const [signals, setSignals] = useState([]);
    const [callTimestamp, setCallTimestamp] = useState(null)
    // const [currentInsight, setCurrentInsight] = useState(null);
    const [callProgress, setCallProgress] = useState("early");
    // const [timerStartTime, setTimerStartTime] = useState(null);
    // const [lastInsightMessage, setLastInsightMessage] = useState('');

    
    
    //REFS
    const isCoachActiveRef = useRef(false);
    const processTranscriptsRef = useRef(null);
    const transcriptionsRef = useRef([]);


    //HOOKS
    const { getIceBreaker, getProspects, processCallSummary } = useCoach()
    const { startLiveCoach, stopLiveCoach } = useAudio()
    const { currentInsight, enqueue, reset, pending } = useInsightScheduler({
        displayMs: 12000,
        minIntervalMs: 5000
    });

    //FUNCTIONS
    const openCoachWindow = () => {
    
        if (window.electron && window.electron.ipcRenderer) {
            window.electron.ipcRenderer.send('open-coach-window');
            setIsCoachWindowOpen(true);
            localStorage.setItem('coach-window-open', 'true'); // Save to localStorage
        } else {
            console.warn('Electron not available, cannot open coach window');
            setIsCoachWindowOpen(false);
            localStorage.setItem('coach-window-open', 'false');
        }
    };

    const closeCoachWindow = () => {
    
        if (window.electron && window.electron.ipcRenderer) {
            window.electron.ipcRenderer.send('close-coach-window');
            setIsCoachWindowOpen(false);
            localStorage.setItem('coach-window-open', 'false'); // Save to localStorage
        } else {
            console.warn('Electron not available, cannot close coach window');
        }
        
        resetCoach()
    };

    const transformTranscriptionsToMessages = (transcriptions) => {
        return transcriptions.map(transcript => ({
            role: "user",
            content: `[${transcript.speaker.toUpperCase()}]: ${transcript.text}`
        }));
    };

    // Keep the calculateCallProgress function but make it work with seconds
    const calculateCallProgress = (elapsedSeconds) => {
        const elapsedMinutes = elapsedSeconds / 60;
        if (elapsedMinutes < 2) return "early";
        if (elapsedMinutes < 7) return "middle";
        return "late";
    };

    const getLast45SecondsOfTranscripts = () => {
        const nowInSeconds = Math.floor(Date.now() / 1000);
        const cutoffTime = nowInSeconds - 45;
        
        console.log(' [getLast45SecondsOfTranscripts] Current time:', nowInSeconds, 'Cutoff:', cutoffTime);
        console.log('🔍 [getLast45SecondsOfTranscripts] Total transcripts in ref:', transcriptionsRef.current.length);
        
        const filtered = transcriptionsRef.current.filter(transcript => {
            let transcriptTime = transcript.timestamp;
            
            if (transcriptTime > 9999999999) {
                transcriptTime = Math.floor(transcriptTime / 1000);
            }
            
            const isRecent = transcriptTime >= cutoffTime;
            
            console.log('🔍 [getLast45SecondsOfTranscripts] Transcript time:', transcriptTime, 'isRecent:', isRecent, 'text:', transcript.text?.substring(0, 50));
            
            return isRecent;
        });
        
        console.log('🔍 [getLast45SecondsOfTranscripts] Filtered recent transcripts:', filtered.length);
        return filtered;
    };
    
    const hasRecentTranscripts = () => {
        const recent = getLast45SecondsOfTranscripts();
        const hasRecent = recent.length > 0;
        console.log('🔍 [hasRecentTranscripts] Has recent transcripts:', hasRecent, 'count:', recent.length);
        return hasRecent;
    };

    

    const fetchProspects = async () => {

        try {
            const prospectsData = await getProspects()
            setProspects(prospectsData)
            
        } catch (error) {
            console.error('Error fetching prospects:', error)
            
        }
    }

    const startCoach = async (prospectId) => {
        setIsCoachLoading(true)
        if(!globalUser) {
            console.log('No global user found')
            return
        }
        if(!prospectId) {
            console.log('No prospect id found')
            return
        }
        console.log('Starting coach with prospect id:', prospectId)

        try {
            const newSessionId = v4();
            setSessionId(newSessionId);
            setProspectId(prospectId);
            
            // ✅ Remove this line - we don't need timerStartTime anymore
            // setTimerStartTime(Date.now());
            
            console.log(`Starting coach for prospect: ${prospectId} with sessionId: ${newSessionId}`)
            const iceBreakerResponse = await getIceBreaker(prospectId);
            if(iceBreakerResponse && iceBreakerResponse.iceBreaker) {
                console.log('Ice breaker response:', iceBreakerResponse)
                console.log('Ice breaker object:', iceBreakerResponse.iceBreaker)
                console.log('Ice breaker message:', iceBreakerResponse.iceBreaker)
                
                // Check if the iceBreaker property exists and has content
                const iceBreakerMessage = iceBreakerResponse.iceBreaker;
                if (iceBreakerMessage && iceBreakerMessage.trim() !== '') {
                    enqueue({
                        message: iceBreakerMessage,
                        isIceBreaker: true,
                        insight: 'yes'
                    });
                } else {
                    console.warn('⚠️ Ice breaker message is empty or undefined');
                }
            } 
            setCallTimestamp(Date.now())
            startLiveCoach({
                accountId: globalUser.id,
                prospectId: prospectId,
                meetingId: null,
                sessionId: newSessionId
            })
            console.log('✅ [startCoach] Setting isCoachActive to true')
            setIsCoachActive(true)
        } catch (error) {
            console.error('Error starting coach:', error)
        } finally {
            setIsCoachLoading(false)
        }
    }

    const resetCoach = () => {
        setSignals([])
        setReceivedInsights([])
        // setCurrentInsight(null)
        setProspectId(null)
        setSessionId(null)
        setLastInsightMessage('')
        setIceBreaker(null)
        setIsCoachActive(false)
        setIsCoachLoading(false)
        setCallDurationInSeconds(0)
        setTranscriptions([])
        setCallTimestamp(null)
    }

    const stopCoach = async () => {
        console.log('🛑 [stopCoach] Stopping coach...')
        setIsCoachLoading(true)
        try {
            console.log('❌ [stopCoach] Setting isCoachActive to false')
            setIsCoachActive(false)
            stopLiveCoach()
            await processCallSummary(sessionId, prospectId, callDurationInSeconds, signals, callTimestamp);
            resetCoach()
        } catch (error) {
            console.error('❌ [stopCoach] Error stopping coach:', error)
        } finally {
            setIsCoachLoading(false)
        }
    }

    const processTranscriptsForInsights = useCallback(async () => {
        
        
        try {
            const accountId = globalUser.id
            console.log('🔍 [processTranscriptsForInsights] Current state:', {
                isCoachActive,
                transcriptionsCount: transcriptions.length,
                prospectId,
                accountId,
                insightsCount: receivedInsights.length
            });
            console.log('🔍 [processTranscriptsForInsights] Signals:', signals);

            // Check if coach is active and we have a prospect ID
            if (!isCoachActiveRef.current || !prospectId) {
                console.log('⚠️ [processTranscriptsForInsights] Coach not active or no prospect ID, skipping insight processing', {
                    isCoachActive: isCoachActiveRef.current,
                    prospectId,
                    stateIsCoachActive: isCoachActive
                });
                return;
            }
            //WE NEED TO GET THE LAST 45 SECONDS OF THE CALL IN TRANSCRIPTIONS
            const recentTranscripts = getLast45SecondsOfTranscripts();
            console.log('🔍 [processTranscriptsForInsights] Recent transcripts count:', recentTranscripts.length);
            
            if (recentTranscripts.length === 0) {
                console.log('⚠️ [processTranscriptsForInsights] No recent transcripts, skipping insight processing');
                return;
            }
            
            //FORMAT THE TRANSCRIPTIONS INTO MESSAGES
            const transformedMessages = transformTranscriptionsToMessages(recentTranscripts);
            const conversationContext = transformedMessages.map(msg => msg.content).join('\n');
            console.log('🔍 [processTranscriptsForInsights] Conversation context length:', conversationContext.length);
            console.log('🔍 [processTranscriptsForInsights] Conversation preview:', conversationContext.substring(0, 200) + '...');

            // Add this right before the runCoach call (around line 216)
            console.log('🚀 [processTranscriptsForInsights] ===== CALLING RUNCOACH =====');
            console.log('🚀 [processTranscriptsForInsights] Parameters being sent to runCoach:', {
                conversationContext: conversationContext.substring(0, 200) + '...',
                insightsCount: receivedInsights.length,
                signalsCount: signals.length,
                callProgress,
                prospectId
            });

            //RUN THE COACH
            console.log('🔍 [processTranscriptsForInsights] Signals:', signals);
            const coachResponse = await runCoach(conversationContext, receivedInsights, signals, callProgress, prospectId);
            console.log('🔍 [processTranscriptsForInsights] ===== RUNCOACH RESPONSE RECEIVED =====');
            console.log('🎯 [processTranscriptsForInsights] Full coach response:', JSON.stringify(coachResponse, null, 2));
            console.log('🚀 [processTranscriptsForInsights] Coach response:', coachResponse);
            console.log('🚀 [processTranscriptsForInsights] Response type check:', {
                isObject: typeof coachResponse === 'object',
                hasInsight: 'Insight' in coachResponse,
                hasMessage: 'Message' in coachResponse,
                insightValue: coachResponse?.Insight,
                messageValue: coachResponse?.Message
            });

            // Only add new detected signals with a valid signal name, and keep the existing ones!
            if(isCoachActiveRef.current) {
                setSignals(prevSignals => {
                    const newSignals = { ...prevSignals };
                    
                    (coachResponse.signals || []).forEach(signal => {
                        if (signal.detected && signal.signal) {
                            newSignals[signal.signal] = {
                                detected: signal.detected,
                                quote: signal.quote
                            };
                        }
                    });
                    
                    return newSignals;
                });

                if(coachResponse.insights?.Insight === 'yes' && coachResponse.insights?.Message !== '') {

                    const newInsightMessage = coachResponse.insights.Message;

                    setReceivedInsights(prev => [...prev, newInsightMessage]);
                  
                    enqueue({
                        message: newInsightMessage,
                        isIceBreaker: false,
                        insight: 'yes'
                    });
                    
                    console.log('✅ [processTranscriptsForInsights] ===== NEW INSIGHT GENERATED! =====');
                    console.log('🎯 [processTranscriptsForInsights] Message:', newInsightMessage);
                } else {
                    console.log('❌ [processTranscriptsForInsights] No insight generated - Conditions not met:', {
                        insightValue: coachResponse.insights?.Insight,
                        messageValue: coachResponse.insights?.Message,
                        insightIsYes: coachResponse.insights?.Insight === 'yes',
                        messageNotEmpty: coachResponse.insights?.Message && coachResponse.insights?.Message !== ''
                    });
                }
            } else {
                console.log('❌ [processTranscriptsForInsights] Coach not active, skipping insight processing');
                return;
            }
            


            
        } catch (error) {
            console.error('❌ [processTranscriptsForInsights] Error processing transcripts:', error);
        } 
        
        console.log('🏁 [processTranscriptsForInsights] ===== INSIGHT PROCESSING COMPLETE =====');
    }, [prospectId, signals, callProgress, enqueue ]);

    // Store the latest version of the function in a ref
    processTranscriptsRef.current = processTranscriptsForInsights;
    
    // Keep the ref in sync with the state
    useEffect(() => {
        isCoachActiveRef.current = isCoachActive;
        console.log('🔄 [useEffect] Updated isCoachActiveRef to:', isCoachActive, 'at', new Date().toISOString());
    }, [isCoachActive]);

    //EFFECTS
    useEffect(() => {
        const globalUser = JSON.parse(localStorage.getItem('sayso-global-user'))
        console.log('globalUser', globalUser)
        setGlobalUser(globalUser)
    }, [])

    useEffect(() => {
        fetchProspects()
    }, [])

    useEffect(() => {
        if(isCoachActive) {
            const interval = setInterval(() => {
                setCallDurationInSeconds(prev => prev + 1)
            }, 1000)
            return () => clearInterval(interval)
        } else {
            setCallDurationInSeconds(0)
        }
    }, [isCoachActive])

    useEffect(() => {
        if (isCoachActive && callDurationInSeconds > 0) {
            const progress = calculateCallProgress(callDurationInSeconds);
            setCallProgress(progress);
            console.log(`🕐 [useEffect] Call progress updated: ${progress} (${callDurationInSeconds}s)`);
        } else if (!isCoachActive) {
            setCallProgress("early"); // Reset to early when coach stops
        }
    }, [callDurationInSeconds, isCoachActive]);

    // Remove the old timer useEffect entirely (lines 463-476)

    // useEffect(() => {
    //     if (!signalReceived || !Array.isArray(signalReceived)) {
    //         return;
    //     }

    //     setSignals(prevSignals => {
    //         const newSignals = { ...prevSignals };
            
    //         signalReceived.forEach(signalItem => {
    //             const { signal, detected } = signalItem;
                
    //             // Only update if the signal is detected (true)
    //             if (detected && (!(signal in prevSignals) || prevSignals[signal] === false)) {
    //                 newSignals[signal] = true;
    //             } else if (!(signal in prevSignals)) {
    //                 newSignals[signal] = false;
    //             }
    //         });
            
    //         return newSignals;
    //     });
    // }, [signalReceived]);
    
    useEffect(() => {
        console.log('🔍 [useEffect] Setting up transcription listener');
        console.log('🔍 [useEffect] Electron available:', !!window.electron);
        console.log('🔍 [useEffect] IPC Renderer available:', !!window.electron?.ipcRenderer);

        if (window.electron && window.electron.ipcRenderer) {
            console.log('🔍 [useEffect] Setting up transcription-data listener');
            
            const handleTranscriptionData = (data) => {
                console.log('🎤 [handleTranscriptionData] Received transcription data:', data);
                console.log('🔍 [handleTranscriptionData] Data type:', typeof data);
                console.log('🔍 [handleTranscriptionData] Data keys:', data ? Object.keys(data) : 'null');
                console.log('🔍 [handleTranscriptionData] Has text property:', data && 'text' in data);
                console.log('🔍 [handleTranscriptionData] Data.text:', data?.text);
                
                if (data && typeof data === 'object' && 'text' in data) {
                    const newTranscription = {
                        text: data.text,
                        speaker: data.speaker || 'unknown',
                        timestamp: data.timestamp 
                    };
                    
                    console.log('🎤 [handleTranscriptionData] Processing new transcription:', {
                        text: newTranscription.text,
                        speaker: newTranscription.speaker,
                        timestamp: newTranscription.timestamp
                    });
                    
                    setTranscriptions(prev => {
                        const updated = [...prev, newTranscription].sort((a, b) => a.timestamp - b.timestamp);
                        // Update the ref with the latest transcriptions
                        transcriptionsRef.current = updated;
                        console.log('🎤 [handleTranscriptionData] Updated transcriptions count:', updated.length);
                        console.log('🎤 [handleTranscriptionData] Updated transcriptions ref count:', transcriptionsRef.current.length);
                        return updated;
                    });
                } else {
                    console.warn('🎤 [handleTranscriptionData] Invalid transcription data received:', data);
                }
            };

            // Add debugging for the listener setup
            console.log('🔍 [useEffect] About to set up listener for transcription-data');
            const cleanup = window.electron.ipcRenderer.on('transcription-data', handleTranscriptionData);
            console.log('🔍 [useEffect] Listener set up, cleanup function:', typeof cleanup);

            return () => {
                console.log('🧹 [useEffect] Cleaning up transcription listener');
                if (cleanup && typeof cleanup === 'function') {
                    cleanup();
                }
            };
        } else {
            console.warn('🎤 [useEffect] Electron not available for transcription listener');
        }
    }, []);
    
    
    
    useEffect(() => {
        console.log('🔍 [useEffect] Centralized insight processing effect triggered:', {
            transcriptionsLength: transcriptions.length,
            hasRecentTranscripts: hasRecentTranscripts(),
            isCoachActive
        });
        
        // Only start processing if we have enough transcripts, recent activity, and coach is active
        if (transcriptions.length > 2 && hasRecentTranscripts() && isCoachActive) {
            console.log('🚀 [useEffect] Conditions met - processing insights');
            processTranscriptsRef.current();
        } else {
            console.log('🔍 [useEffect] Conditions not met for insight processing:', {
                transcriptionsLength: transcriptions.length,
                hasRecentTranscripts: hasRecentTranscripts(),
                isCoachActive
            });
        }

        // Remove the problematic cleanup function - it's causing the issue!
        // return () => {
        //     if (!isCoachActive) {
        //         resetCoach()
        //     }
        // };
    }, [transcriptions, isCoachActive]);

    // Remove the old timer useEffect entirely (lines 463-476)

    useEffect(() => {
        if (window.electron && window.electron.ipcRenderer) {
            const handleCoachWindowClosed = () => {
                console.log('🎯 [CoachWindowContext] Coach window closed event received');
                setIsCoachWindowOpen(false);
                localStorage.setItem('coach-window-open', 'false'); // Save to localStorage
            };

            window.electron.ipcRenderer.on('coach-window-closed', handleCoachWindowClosed);

            return () => {
                window.electron.ipcRenderer.removeAllListeners('coach-window-closed');
            };
        }
    }, []);

    const value = {
        prospects,
        startCoach,
        stopCoach,
        setProspectId,
        iceBreaker,
        isCoachActive,
        callDurationInSeconds,
        isCoachLoading, 
        isCoachWindowOpen,
        openCoachWindow,
        closeCoachWindow,
        currentInsight,
        signals,
        setSignals
    }

    return (
        <CoachWindowContext.Provider value={value}>
            {children}
        </CoachWindowContext.Provider>
    );
};

export const useCoachWindowContext = () => {
  const context = useContext(CoachWindowContext);
  if (!context) {
    throw new Error('useCoachWindowContext must be used within CoachWindowProvider');
  }
  return context;
};
