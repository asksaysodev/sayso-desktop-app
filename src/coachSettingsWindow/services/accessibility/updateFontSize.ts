import { AccessibilityFontSizeType } from "@/coachSettingsWindow/types";
import apiClient from "@/config/axios";

export default async function updateFontSize(size: AccessibilityFontSizeType): Promise<unknown> {
    const response = await apiClient.post(`sales-coach/settings/font-size`, { fontSize: size });

    if (!response?.data) {
        throw new Error('Failed to update font size');
    }

    return response.data;
}