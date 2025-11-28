import { createContext, useContext, useState, useEffect } from 'react';
import { v4 } from 'uuid';

import useCoach from '../coachWindow/hooks/useCoach';
import { useAudioUpload } from '../coachWindow/hooks/useAudioUpload';
import useCue from '../coachWindow/hooks/useCue';

const CoachWindowContext = createContext();

let openWindowCheckInterval = null;
let openWindowTimeoutId = null;

export const CoachWindowProvider = ({ children }) => {

    //STATE
    const [isCoachWindowOpen, setIsCoachWindowOpen] = useState(false)
    const [prospects, setProspects] = useState([])
    const [selectedProspect, setSelectedProspect] = useState(null)
    const [prospectId, setProspectId] = useState(null)
    const [iceBreaker, setIceBreaker] = useState(null)
    const [globalUser, setGlobalUser] = useState(null)
    const [isCoachActive, setIsCoachActive] = useState(false)
    const [isCoachLoading, setIsCoachLoading] = useState(false)
    const [coachFeature, setCoachFeature] = useState('cue') // 'cue' or 'recall' FEATURE_FLAG_RECALL || FEATURE_FLAG_CUE_REAL_ESTATE
    // const [callDurationInSeconds, setCallDurationInSeconds] = useState(0)
    const [sessionData, setSessionData] = useState(null)
    const [signals, setSignals] = useState([])

    //HOOKS
    const { getProspects } = useCoach()
    const { compressAudioFile, uploadFullRecording, isCompressing, isUploading } = useAudioUpload()
    const { currentInsight, insightsQueue, showNext, addInsight } = useCue();
    
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
    
    /**
    * @param {boolean} isOpen
    */
    function updateCoachWindowOpenStates(isOpen) {
        setIsCoachWindowOpen(isOpen);
    }

    const openCoachWindow = () => {
        return new Promise((resolve, reject) => {
            const tryOpen = () => {
                if (window.electron?.ipcRenderer) {
                    window.electron.ipcRenderer.send('open-coach-window');
                    updateCoachWindowOpenStates(true);
                    return true;
                }
                return false;
            };
            
            if (tryOpen()) {
                resolve(true);
                return;
            }

            console.log('⏳ [CoachWindowContext] Electron not ready, retrying...');
            clearInterval(openWindowCheckInterval);
            clearTimeout(openWindowTimeoutId);
            
            openWindowCheckInterval = setInterval(() => {
                if (tryOpen()) {
                    clearInterval(openWindowCheckInterval);
                    clearTimeout(openWindowTimeoutId);
                    console.log('[CoachWindowContext] Coach window opened');
                    resolve(true);
                }
            }, 50);
            
            openWindowTimeoutId = setTimeout(() => {
                clearInterval(openWindowCheckInterval);
                if (!window.electron?.ipcRenderer) {
                    console.error('[CoachWindowContext] Electron unavailable after timeout');
                    updateCoachWindowOpenStates(false);
                    reject(new Error('Electron not available - timeout after 1s'));
                }
            }, 1000);
        });
    };

    const closeCoachWindow = () => {
        clearInterval(openWindowCheckInterval);
        clearTimeout(openWindowTimeoutId);

        if (window.electron && window.electron.ipcRenderer) {
            window.electron.ipcRenderer.send('close-coach-window');
            updateCoachWindowOpenStates(false)
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
        if (window.electron && window.electron.ipcRenderer) {
            const handleCoachWindowClosed = () => {
                console.log('🎯 [CoachWindowContext] Coach window closed event received');
                updateCoachWindowOpenStates(false)
            };

            window.electron.ipcRenderer.on('coach-window-closed', handleCoachWindowClosed);

            return () => {
                window.electron.ipcRenderer.removeAllListeners('coach-window-closed');
            };
        }
    }, []);

    

    const value = {
        prospects,
        selectedProspect,
        setSelectedProspect,
        setProspectId,
        iceBreaker,
        isCoachActive,
        setIsCoachActive,
        isCoachLoading, 
        isCoachWindowOpen,
        updateCoachWindowOpenStates,
        openCoachWindow,
        closeCoachWindow,
        currentInsight,
        insightsQueue,
        showNext,
        addInsight,
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
        coachFeature,
        setCoachFeature,
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
    console.log('useCoachWindowContext must be used within CoachWindowProvider')
    throw new Error('useCoachWindowContext must be used within CoachWindowProvider');
  }
  return context;
};
