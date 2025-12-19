import apiClient from "@/config/axios";
import { Signal } from "../types";

export default async function getSignals(leadType: string): Promise<Signal[]> {
    const response = await apiClient.get(`/admin/cue/signals?leadType=${leadType}`);

    if (!response?.data) {
        throw new Error('Failed to fetch signals');
    }

    return response.data;
}