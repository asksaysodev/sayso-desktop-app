import apiClient from "@/config/axios";

export default async function getStripeCheckoutPageUrl(stripePriceId) {
    const response = await apiClient.post(`/payment/checkout/stripe`, { stripePriceId });
    return response.data;
}