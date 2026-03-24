import apiClient from "@/config/axios";
import { CueMode } from "../../types";

export default async function updateCueMode(mode: CueMode): Promise<unknown> {
    const response = await apiClient.post(`sales-coach/settings/cue-mode`, {cueMode: mode});

    if (!response?.data) {
        throw new Error('Failed to update cue mode');
    }

    return response.data;
}