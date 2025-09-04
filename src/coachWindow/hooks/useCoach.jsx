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

    const processCallSummary = useCallback(async (sessionId, prospectId) => {

        try {
          if(!sessionId || !prospectId) {
            throw new Error('Session ID and prospect ID are required');
          }
    
          const data = {
            sessionId,
            prospectId
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