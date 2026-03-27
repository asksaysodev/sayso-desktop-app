import apiClient from "@/config/axios";
import { PostAutoStopTimeDelayResponse } from "../../types";

export default async function postAutoStopTimeDelay(delay: number): Promise<PostAutoStopTimeDelayResponse> {
    if (delay === undefined ||delay === null) {
         throw new Error('Property delay is neither string nor number');
    }
    
    const response = await apiClient.post(`sales-coach/settings/auto-stop-delay`, { autoStopDelay: delay });

    if (!response?.data) {
        throw new Error('Failed to post auto stop time delay');
    }

    return response.data;
}