import apiClient from "@/config/axios";

export default async function getWeeklyActivity(weekOffset = 0) {
    const response = await apiClient.get(`accounts/activity/weekly?weekOffset=${weekOffset}`);

    if (!response?.data) {
        throw new Error('Failed to fetch weekly activity');
    }

    return response?.data
}