import { useState } from "react";

import apiClient from "../../config/axios";
import { supabase } from "../../config/supabase";

export default function useCue() {
    
    //STATE
    const [insights, setInsights] = useState([]);
    const [sessionId, setSessionId] = useState(null);
    const [isCueActive, setIsCueActive] = useState(false);
    const [scenario, setScenario] = useState('buyer');
    const [isLoading, setIsLoading] = useState(false);

    //FUNCTIONS
    const handleStartCue =async () => {
        setIsLoading(true);
        try {
            if(isCueActive) {
                throw new Error('Cue is already active');
            }
            if(!scenario) {
                throw new Error('Scenario is required');
            }
            if(scenario !== 'buyer' && scenario !== 'seller') {
                throw new Error('Scenario must be either "buyer" or "seller"');
            }
            const sessionResponse = await createNewCueSession(scenario);
            if(!sessionResponse || !sessionResponse.sessionId) {
                throw new Error('Failed to create new cue session');
            }
            setSessionId(sessionResponse.sessionId);
            await startCueStreaming(sessionResponse.sessionId);
            setIsCueActive(true);
        } catch (error) {
            console.error('Error starting cue:', error);
            throw error;
        } finally {
            setIsLoading(false);
        }
    }

    const createNewCueSession = async (scenario) => {
        try {
            if(!scenario) {
                throw new Error('Scenario is required');
            }
            if(scenario !== 'buyer' && scenario !== 'seller') {
                throw new Error('Scenario must be either "buyer" or "seller"');
            }
            const payload = {
                data: {
                    scenario
                }
            }
            const response = await apiClient.post('/cue/session/new', payload);
            return response.data;
        } catch (error) {
            console.error('Error creating new cue session:', error);
            throw error;
        }
    }

    const handleSetScenario = (scenario) => {
        if(!scenario) {
            throw new Error('Scenario is required');
        }
        if(scenario !== 'buyer' && scenario !== 'seller') {
            throw new Error('Scenario must be either "buyer" or "seller"');
        }
        setScenario(scenario);
    }

    const handleStopCue = async (sessionId) => {
        setIsLoading(true);
        try {
            if(!sessionId) {
                throw new Error('Session ID is required');
            }
            await window.electron.cue.stop();
        } catch (error) {
            console.error('Error stopping cue:', error);
            throw error;
        } finally {
            resetCueStates();
            setIsLoading(false);
        }
    }

    const startCueStreaming = async (sessionId) => {
        try {
            if(!sessionId) {
                throw new Error('Session ID is required');
            }
            
            if (!window.electron?.cue) {
                throw new Error('Electron cue API not available');
            }
            
            const { data: {session}} = await supabase.auth.getSession();
            
            if(!session || !session.access_token) {
                throw new Error('No session found');
            }

            const result = await window.electron.cue.start({ 
                sessionId, 
                token: session.access_token 
            });

            if(!result || !result.success) {
                throw new Error('Failed to start cue streaming');
            }
            return result;
        } catch (error) {
            console.error('Error starting cue streaming:', error);
            throw error;
        }
    }

    const resetCueStates = () => {
        setIsCueActive(false);
        setSessionId(null);
        setInsights([]);
        setScenario(null);
    }

    return {
        insights,
        sessionId,
        isCueActive,
        scenario,
        handleStartCue,
        createNewCueSession,
        handleSetScenario,  
        handleStopCue,
        isLoading
    }
}