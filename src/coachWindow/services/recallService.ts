import apiClient from "../../config/axios"

export const getIceBreaker = async (prospectId: string) => {
    try {
        const response = await apiClient.get(`/sales-coach/ice-breaker/${prospectId}`); 
        console.log('response', response)
        return response.data.iceBreaker;
    } catch (error) {
        console.error('Error getting ice breaker:', error)
    }
}

export const getProspects = async () => {
    try {
        const response = await apiClient.get('/prospects/account-prospects');
        return response.data.data;
    } catch (error) {
        console.error('Error starting coach:', error)
    }
}

