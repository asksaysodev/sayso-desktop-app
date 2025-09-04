import { createContext, useContext, useState, useEffect } from 'react';
import { v4 } from 'uuid';
import useCoach from '../coachWindow/hooks/useCoach';
import useAudio from '../coachWindow/hooks/useAudio';

const CoachWindowContext = createContext();

export const CoachWindowProvider = ({ children }) => {

    //STATE
    const [prospects, setProspects] = useState([])
    const [prospectId, setProspectId] = useState(null)
    const [sessionId, setSessionId] = useState(null)
    const [iceBreaker, setIceBreaker] = useState(null)
    const [globalUser, setGlobalUser] = useState(null)
    const [isCoachActive, setIsCoachActive] = useState(false)
    const [isCoachLoading, setIsCoachLoading] = useState(false)

    //HOOKS
    const { getIceBreaker, getProspects, processCallSummary } = useCoach()
    const { startLiveCoach, stopLiveCoach } = useAudio()

    //EFFECTS
    useEffect(() => {
        const globalUser = JSON.parse(localStorage.getItem('sayso-global-user'))
        console.log('globalUser', globalUser)
        setGlobalUser(globalUser)
    }, [])

    const fetchProspects = async () => {

        try {
            const prospectsData = await getProspects()
            setProspects(prospectsData)
            
        } catch (error) {
            console.error('Error fetching prospects:', error)
            
        }
    }

    const startCoach = async () => {
        setIsCoachLoading(true)
        if(!globalUser) {
            console.log('No global user found')
            return
        }
        console.log('Starting coach')

        try {
            const newSessionId = v4();
            setSessionId(newSessionId);
            console.log(`Starting coach for prospect: ${prospectId} with sessionId: ${newSessionId}`)
            const iceBreakerResponse = await getIceBreaker(prospectId);
            if(iceBreakerResponse) {
                setIceBreaker(iceBreakerResponse.iceBreaker);
            } 
            startLiveCoach({
                accountId: globalUser.id,
                prospectId: prospectId,
                meetingId: null,
                sessionId: newSessionId
            })
            setIsCoachActive(true)
        } catch (error) {
            console.error('Error starting coach:', error)
        } finally {
            setIsCoachLoading(false)
        }
    }

    const stopCoach = async () => {
        try {
            setIsCoachActive(false)
            stopLiveCoach()
            await processCallSummary(sessionId, prospectId);
            setProspectId(null)
            setSessionId(null)
            setIceBreaker(null)
        } catch (error) {
            console.error('Error stopping coach:', error)
        }
    }

    useEffect(() => {
        fetchProspects()
    }, [])

    const value = {
        prospects,
        startCoach,
        stopCoach,
        setProspectId,
        iceBreaker,
        isCoachActive,
        setIsCoachActive
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
