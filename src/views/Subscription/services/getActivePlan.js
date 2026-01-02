import apiClient from "@/config/axios";

export default async function getActivePlan() {
    const response = await apiClient.get('/accounts/subscription');
    
    if (!response?.data) {
        throw new Error('Failed to fetch active plan');
    }

    return response?.data
}