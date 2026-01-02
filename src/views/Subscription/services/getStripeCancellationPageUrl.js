import apiClient from "@/config/axios";

export default async function getStripeCancellationPageUrl(stripePriceId) {
    const response = await apiClient.post(`/payment/stripe/customer-portal`, { stripePriceId });
    if (!response?.data) {
        throw new Error('Failed to fetch stripe cancellation page url');
    }
    return response.data;
}