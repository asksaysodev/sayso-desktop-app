import apiClient from "@/config/axios";

/**
 * 
 * @returns {Object} - Account usage data
 * @property {number} planMinutes
 * @property {number} remainingMinutes
 * @property {number} rolloverMinutes
 * @property {number} usedMinutes
 */
export default async function getAccountUsage() {
    const response = await apiClient.get('/accounts/usage');

    if (!response?.data) {
        throw new Error('Failed to fetch account usage');
    }

    return response?.data
}