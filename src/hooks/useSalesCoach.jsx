import { useCallback, useState } from 'react';
import { useAuth } from '../context/AuthContext';
import { useAudioCapture } from './useAudioCapture';
import { useSalesCoachContext } from '../context/SalesCoachContext';
import axios from 'axios';

export const useSalesCoach = () => {

  const { globalUser, authToken } = useAuth();
  const { startLiveCoach, stopLiveCoach } = useAudioCapture();
  const { setIsCallActive, setIceBreaker, setCurrentInsight, setProspectId, setAccountId } = useSalesCoachContext();

  const getIceBreaker = useCallback(async (prospectId) => {
    try {
      const response = await axios.get(`${import.meta.env.VITE_BACKEND_BASE_URL}/sales-coach/ice-breaker/${prospectId}`, {
        headers: {
          'Content-Type': 'application/json',
          'Authorization': `Bearer ${authToken}`
        }
      });
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
      
      startLiveCoach({ accountId, prospectId, meetingId });
      setIsCallActive(true);
      return;
      
    } catch (error) {
      setIsCallActive(false);
      stopLiveCoach();
      console.error('Error in handleNewCall:', error);
      throw error;
    }
  }, [globalUser, getIceBreaker, setIceBreaker, startLiveCoach, setIsCallActive]);


  const getCallSummary = useCallback(async (meetingId, prospectId) => {

    try {
      if(!meetingId) {
        throw new Error('Meeting ID is required');
      }
      if(!prospectId) {
        throw new Error('Prospect ID is required');
      }
  
      if (!authToken) {
        throw new Error('Authentication token is required');
      }

      const data = {
        meetingId,
        prospectId
      }

      const response = await axios.post(
        `${import.meta.env.VITE_BACKEND_BASE_URL}/sales-coach/call-summary`,
        data,
        {
          headers: {
            'Content-Type': 'application/json',
            'Authorization': `Bearer ${authToken}`
          }
        }
      );

      return response.data.summary;
    } catch (error) {
      console.error('Error in getCallSummary:', error);
    }
    
  }, [authToken]);

  

  const handleStopLiveCoach = useCallback(async (meetingId, prospectId) => {
    stopLiveCoach();
    setIsCallActive(false);
    const summary = await getCallSummary(meetingId, prospectId);
  }, [stopLiveCoach, setIsCallActive, getCallSummary]);



  return {
    handleNewCall,
    startLiveCoach,
    handleStopLiveCoach,
    getCallSummary,
    getIceBreaker,
  };
};
