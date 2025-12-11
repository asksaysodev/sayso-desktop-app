import apiClient from "@/config/axios";

export default async function getInsights(page = 0) {
    const response = await apiClient.get(`sales-coach/insights/history?page=${page}`);

    if (!response?.data) {
        throw new Error('Failed to fetch insights');
    }

    return response?.data
}