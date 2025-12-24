import apiClient from "@/config/axios";

export default async function getSubscriptionPlans() {
    const response = await apiClient.get('/pricing/plans');
    
    if (!response?.data) {
        throw new Error('Failed to fetch subscription plans');
    }

    return response?.data
}