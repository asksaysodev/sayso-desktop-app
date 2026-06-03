import apiClient from "@/config/axios";
import { PostOpenLastUsedResponse } from "../../types";

export default async function postOpenLastUsed(openLastUsed: boolean): Promise<PostOpenLastUsedResponse> {
    const response = await apiClient.post(`sales-coach/settings/open-last-used`, { openLastUsed });

    if (!response?.data) {
        throw new Error('Failed to update open last used setting');
    }

    return response.data;
}
