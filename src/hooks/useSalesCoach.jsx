import { useCallback, useState } from 'react';
import { useAuth } from '../context/AuthContext';
import { useAudioCapture } from './useAudioCapture';
import { useSalesCoachContext } from '../context/SalesCoachContext';
import apiClient from '../config/axios';
import { v4 } from 'uuid';

export const useSalesCoach = () => {

  const { globalUser } = useAuth();
  const { startLiveCoach, stopLiveCoach } = useAudioCapture();
  const { setIsCallActive, setIceBreaker, setCurrentInsight, setProspectId, setAccountId, setSessionId } = useSalesCoachContext();

  const getIceBreaker = useCallback(async (prospectId) => {
    try {
      const response = await apiClient.get(`/sales-coach/ice-breaker/${prospectId}`);
      return response.data.iceBreaker;
    } catch (error) {
      console.error('Error in getIceBreaker:', error);
      throw error;
    }
  }, []);


  const handleNewCall = useCallback(async (prospectId, accountId, meetingId) => {
    if (!globalUser?.id) {
      throw new Error('Global user not found');
    }

    if (!prospectId) {
      throw new Error('Prospect ID is required');
    }

    try {

      setProspectId(prospectId);
      setAccountId(accountId);

      const iceBreaker = await getIceBreaker(prospectId);

      if(iceBreaker) {

        const iceBreakerMessage = {
          message: iceBreaker.iceBreaker,
          isIceBreaker: true,
        }
        setCurrentInsight(iceBreakerMessage);

      }
      const newSessionId = v4();
      setSessionId(newSessionId);
      startLiveCoach({ accountId, prospectId, meetingId, sessionId: newSessionId });
      setIsCallActive(true);
      return;
      
    } catch (error) {
      setIsCallActive(false);
      stopLiveCoach();
      console.error('Error in handleNewCall:', error);
      throw error;
    }
  }, [globalUser, getIceBreaker, setIceBreaker, startLiveCoach, setIsCallActive]);


  const getCallSummary = useCallback(async (meetingId, prospectId, sessionId) => {

    try {

      const accountId = globalUser?.id;

      if(!meetingId || !prospectId || !accountId) {
        throw new Error('Meeting ID, prospect ID, and account ID are required');
      }

      const data = {
        meetingId,
        prospectId,
        accountId,
        sessionId
      }

      const response = await apiClient.post(
        '/sales-coach/call-summary',
        data,
      );

      return response.data.summary;
    } catch (error) {
      console.error('Error in getCallSummary:', error); 
    }
    
  }, []);

  

  const handleStopLiveCoach = useCallback(async (meetingId, prospectId, sessionId) => {
    setSessionId(null);
    stopLiveCoach();
    setIsCallActive(false);
    await getCallSummary(meetingId, prospectId, sessionId);
    // Removed window.location.reload() to prevent Electron app issues
  }, [stopLiveCoach, setIsCallActive, getCallSummary]);

  const handleRestartApp = useCallback(async () => {
    window.location.reload();
  }, []);



  return {
    handleNewCall,
    // startLiveCoach,
    handleStopLiveCoach,
    getCallSummary,
    getIceBreaker,
    handleRestartApp,
  };
};
