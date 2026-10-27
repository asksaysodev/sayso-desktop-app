import apiClient from "@/config/axios";
import { PostCallDetectionPromptEnabledResponse } from "../../types";

export default async function postCallDetectionPromptEnabled(callDetectionPromptEnabled: boolean): Promise<PostCallDetectionPromptEnabledResponse> {
    const response = await apiClient.post(`sales-coach/settings/call-detection-prompt-enabled`, { callDetectionPromptEnabled });

    if (!response?.data) {
        throw new Error('Failed to update call detection prompt setting');
    }

    return response.data;
}
