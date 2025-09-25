import { useCallback } from "react"

import apiClient from "../../config/axios"

export default function useCoach() {

    const getIceBreaker = async (prospectId) => {
        try {
            const response = await apiClient.get(`/sales-coach/ice-breaker/${prospectId}`); 
            console.log('response', response)
            return response.data.iceBreaker;
        } catch (error) {
            console.error('Error getting ice breaker:', error)
        }
    }

    const getProspects = async () => {
        try {
            const response = await apiClient.get('/prospects/account-prospects');
            return response.data.data;
            
        } catch (error) {
            console.error('Error starting coach:', error)
        }
    }

    const processCallSummary = useCallback(async (sessionId, prospectId, callDurationInSeconds, signals, insights, callTimestamp) => {
        console.log('🔍 [processCallSummary] Session ID:', sessionId);
        try {
          if(!sessionId) {
            throw new Error('Session ID is required');
          }
          if(!prospectId) {
            throw new Error('Prospect ID is required');
          }
          if(!callDurationInSeconds) {
            throw new Error('Call duration in seconds is required');
          }
          if(!signals) {
            throw new Error('Signals are required');
          }
          if(!insights) {
            throw new Error('Insights are required');
          }
          if(!callTimestamp) {
            throw new Error('Call timestamp is required');
          }
    
          const data = {
            sessionId,
            prospectId,
            callDurationInSeconds,
            signals,
            insights,
            callTimestamp
          }
    
          const response = await apiClient.post(
            '/sales-coach/call-summary',
            data,
          );
    
          return response.data.summary;
        } catch (error) {
          console.error('Error in processCallSummary:', error);
        }
        
      }, []);


    return {
        getIceBreaker,
        getProspects,   
        processCallSummary
    }
}