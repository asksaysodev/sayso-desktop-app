import { createContext, useContext, useState, useEffect, useRef, useCallback } from 'react';
import { runCoach } from '../services/coachServices';
import { v4 } from 'uuid';

import useCoach from '../coachWindow/hooks/useCoach';
import useInsightScheduler from '../coachWindow/hooks/useInsightScheduler';
import { useAudioUpload } from '../coachWindow/hooks/useAudioUpload';

const CoachWindowContext = createContext();

export const CoachWindowProvider = ({ children }) => {

    //STATE
    const [isCoachWindowOpen, setIsCoachWindowOpen] = useState(false)
    const [prospects, setProspects] = useState([])
    const [prospectId, setProspectId] = useState(null)
    const [iceBreaker, setIceBreaker] = useState(null)
    const [globalUser, setGlobalUser] = useState(null)
    const [isCoachActive, setIsCoachActive] = useState(false)
    const [isCoachLoading, setIsCoachLoading] = useState(false)
    const [callDurationInSeconds, setCallDurationInSeconds] = useState(0)
    const [sessionData, setSessionData] = useState(null)
    const [signals, setSignals] = useState([]);
    
    //HOOKS
    const { getProspects } = useCoach()
    const { compressAudioFile, uploadFullRecording, isCompressing, isUploading } = useAudioUpload()
    
    const currentInsight = null;

    //AUDIO FUNCTIONS
    const startDualChannelRecording = async (prospectId) => {
        setIsCoachLoading(true)
        console.log('🎤 [CoachWindowContext] Starting dual channel recording with prospectId:', prospectId);
        
        // Create session data to get sessionId and prospectId
        const sessionData = createNewSessionData(prospectId);
        
        const recordingParams = {
            sessionId: sessionData.sessionId,
            prospectId: prospectId,
            metadata: {
                sessionId: sessionData.sessionId,
                prospectId: sessionData.prospectId,
                timestamp: sessionData.timestamp
            }
        };
        
        console.log('🎤 [CoachWindowContext] Recording params:', recordingParams);
        
        if (window.electron && window.electron.recording) {
            try {
                const result = await window.electron.recording.startDualChannel(recordingParams);
                console.log('🎤 [CoachWindowContext] ✅ Recording started successfully:', result);
                return result;
            } catch (error) {
                console.error('🎤 [CoachWindowContext] ❌ Error starting recording:', error);
                throw error;
            }
            finally {
                setIsCoachLoading(false)
                setIsCoachActive(true)
            }
        } else {
            console.error('🎤 [CoachWindowContext] ❌ Electron recording API is not available!');
            console.error('🎤 [CoachWindowContext] window.electron:', window.electron);
            throw new Error('Electron recording API not available');
        }
    };

    const stopDualChannelRecording = async () => {
        console.log('🎤 [CoachWindowContext] Stopping dual channel recording...');
        
        if (window.electron && window.electron.recording) {
            try {
                const result = await window.electron.recording.stopDualChannel();
                console.log('🎤 [CoachWindowContext] ✅ Recording stopped successfully:', result);
                return result;
            } catch (error) {
                console.error('🎤 [CoachWindowContext] ❌ Error stopping recording:', error);
                throw error;
            }
        } else {
            console.warn('🎤 [CoachWindowContext] Electron recording API is not available. Cannot stop recording.');
            throw new Error('Electron recording API not available');
        }
    };

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
        
        // resetCoach()
    };

    const resetCoach = () => {
        console.log('🎤 [CoachWindowContext] Resetting coach...')
        setIsCoachActive(false)
        setIsCoachLoading(false)
        setCallDurationInSeconds(0)
        setSessionData(null)
        setSignals([])
    }

    const fetchProspects = async () => {
        
        try {
            const prospectsData = await getProspects()
            setProspects(prospectsData)
            
        } catch (error) {
            console.error('Error fetching prospects:', error)
            
        }
    }

    const createNewSessionData = (prospectId) => {
        const newSessionId = v4()
        const newSessionData = {
            sessionId: newSessionId,
            prospectId: prospectId,
            timestamp: Date.now()
        }
        setSessionData(newSessionData)
        return newSessionData
    }

    const handleStopRecording = async () => {

        try {
            setIsCoachLoading(true)
            setIsCoachActive(false)
            const result = await stopDualChannelRecording()
            console.log('🎤 [CoachWindowContext] ✅ Recording stopped successfully:', result)
            const uploadResults = await uploadFullRecording(result)
            console.log('🎤 [CoachWindowContext] ✅ Files uploaded successfully:', uploadResults)
            return result
        } catch (error) {
            console.error('🎤 [CoachWindowContext] ❌ Error stopping recording:', error)
            throw error
        }
        finally {
            setIsCoachLoading(false)
            resetCoach()
        }
    }

    //EFFECTS
    useEffect(() => {
        const globalUser = JSON.parse(localStorage.getItem('sayso-global-user'))
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
        setProspectId,
        iceBreaker,
        isCoachActive,
        setIsCoachActive,
        callDurationInSeconds,
        isCoachLoading, 
        isCoachWindowOpen,
        openCoachWindow,
        closeCoachWindow,
        currentInsight,
        signals,
        setSignals,
        createNewSessionData,   
        sessionData,
        startDualChannelRecording,
        handleStopRecording,
        compressAudioFile,
        uploadFullRecording,
        isCompressing,
        isUploading,
        resetCoach,
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
