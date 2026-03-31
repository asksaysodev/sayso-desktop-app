import apiClient from "@/config/axios";

interface GetAccountSubscriptionResponse {
    subscription?: {
        status: string;
        plan_id?: string;
        current_period_end?: string;
        cancel_at_period_end?: boolean;
    };
}

export default async function getActivePlan(): Promise<GetAccountSubscriptionResponse> {
    const response = await apiClient.get('/accounts/subscription');

    if (!response?.data) {
        throw new Error('Failed to fetch active plan');
    }

    return response?.data;
}
