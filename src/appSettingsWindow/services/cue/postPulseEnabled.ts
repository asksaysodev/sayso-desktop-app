import apiClient from "@/config/axios";
import { PostPulseEnabledResponse } from "../../types";

export default async function postPulseEnabled(pulseEnabled: boolean): Promise<PostPulseEnabledResponse> {
    const response = await apiClient.post(`sales-coach/settings/pulse-enabled`, { pulseEnabled });

    if (!response?.data) {
        throw new Error('Failed to update pulse enabled setting');
    }

    return response.data;
}
