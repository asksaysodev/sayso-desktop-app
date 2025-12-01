import { useState, useEffect, useCallback } from "react";

import apiClient from "../../config/axios";
import { supabase } from "../../config/supabase";

const PRIORITY_ORDER = {
    high: 3,
    mid: 2,
    low: 1,
};

const config = {
    /** Display duration of the toast */
    displayDuration: 6000,
    /** Time between toasts - allows exit animation (300ms) to complete with buffer */
    transitionDelay: 500, 
    /** Time until the toast expires */
    expirationTime: 30000,
    /** Animation duration of the toast */
    animationDuration: 300,
    /** Time until the toast is considered too old to display */
    maxAgeBeforeDisplay: 90000, // 90s
};

export default function useCue() {
    
    //STATE
    const [insightsQueue, setInsightsQueue] = useState([]);
    const [currentInsight, setCurrentInsight] = useState(null);
    const [sessionId, setSessionId] = useState(null);
    const [isCueActive, setIsCueActive] = useState(false);
    const [scenario, setScenario] = useState('buyer');
    const [isLoading, setIsLoading] = useState(false);
    const [isDisplaying, setIsDisplaying] = useState(false);

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
        setInsightsQueue([]);
        setScenario(null);
    }


    /** For handling cases where the toast has expiresAt property and is expired */
    const removeExpired = (queue) => {
        const now = Date.now();
        return queue.filter(insight => insight.expiresAt && insight.expiresAt > now);
    };

    /** Handle cases where the toast has lived longer than maxAgeBeforeDisplay - meaning it should not be displayed */
    const removeTooOld = (queue) => {
        const now = Date.now();
        return queue.filter(insight => {
            const age = now - insight.createdAt;
            return age <= config.maxAgeBeforeDisplay;
        });
    };

    const sortByPriority = (queue) => {
        return [...queue].sort((a, b) => PRIORITY_ORDER[b.priority] - PRIORITY_ORDER[a.priority]);
    };

    const addInsight = useCallback((insight) => {
        const newInsight = {
            ...insight,
            // This we should rethink it. Because, do we want to set the timestamp here at the frontend?
            // because imagine that we receive the Cue/Insight from the backend but for some reason it's an old one.
            // we well treat it as a new one and it will be displayed. The timestamp i believe should be set at the backend.
            createdAt: insight.createdAt || Date.now(),
            // SAme with this maybe. This I also believe is not even necessary. Because
            // we already have the createdAt property. Think more about this.
            expiresAt: insight.expiresAt || Date.now() + config.expirationTime,
        };
        setInsightsQueue(prevQueue => {
            const validQueue = removeExpired(prevQueue);
            const notTooOldQueue = removeTooOld(validQueue);
            const newQueue = [...notTooOldQueue, newInsight];
            return sortByPriority(newQueue);
        });
    }, []);

    const showNext = useCallback(() => {
        setInsightsQueue(prevQueue => {
            const validQueue = removeExpired(prevQueue);
            const notTooOldQueue = removeTooOld(validQueue);
            const sortedQueue = sortByPriority(notTooOldQueue);
            
            // Si ya filtramos los demasiado viejos, el siguiente toast es válido
            const nextInsight = sortedQueue.length > 0 ? sortedQueue[0] : null;
            setCurrentInsight(nextInsight);
            setIsDisplaying(nextInsight !== null);
            
            return nextInsight ? sortedQueue.slice(1) : [];
        });
    }, []);

    // Auto-trigger showNext when queue has items and nothing is currently displaying
    useEffect(() => {
        if (insightsQueue.length > 0 && !isDisplaying && !currentInsight) {
            showNext();
        }
    }, [insightsQueue, isDisplaying, currentInsight]);

    return {
        insightsQueue,
        sessionId,
        isCueActive,
        scenario,
        handleStartCue,
        createNewCueSession,
        handleSetScenario,  
        handleStopCue,
        isLoading,
        currentInsight, 
        setCurrentInsight,
        showNext,
        addInsight
    }
}