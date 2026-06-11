import apiClient from "../../config/axios"
import { Prospect } from "@/types/coach";

export default function useCoach() {

    const getIceBreaker = async (prospectId: string): Promise<string | null> => {
      try {
        const response = await apiClient.get(`/sales-coach/ice-breaker/${prospectId}`); 
        return response.data.iceBreaker?.iceBreaker || null;
      } catch (error) {
        console.error('Error getting ice breaker:', error)
        return null;
      }
    }

    const getProspects = async (): Promise<Prospect[]> => {
        try {
            const response = await apiClient.get('/prospects/account-prospects');
            return response.data.data;
            
        } catch (error) {
            console.error('Error starting coach:', error)
            throw error;
        }
    }

    return {
        getIceBreaker,
        getProspects,
    }
}