import apiClient from "@/config/axios";

export interface MarketDataItem {
    key: string;
    label: string;
    display: boolean;
    value: string | number | { direction: 'up' | 'down' | 'neutral'; changePercent: number } | null;
    detail?: string;
}

export interface PropertyTypeResult {
    type: string;
    data: MarketDataItem[];
}

export interface PulseResponse {
    location: {
        zipCode: string;
        city: string;
        state: string;
    };
    byPropertyType: Record<string, PropertyTypeResult>;
    disclaimer: string;
}

export interface PulseApiError {
    status: number;
    message: string;
}

export default async function getPulseMarketProperty(zipCode: string, sessionId: string): Promise<PulseResponse> {
    try {
        const response = await apiClient.post('/pulse/market/all', { zipCode, sessionId });

        if (!response?.data) {
            throw { status: 500, message: 'Failed to fetch market data' } as PulseApiError;
        }

        return response.data;
    } catch (err: unknown) {
        const e = err as Record<string, unknown>;
        if (e?.status && e?.message) throw err;
        const axiosErr = e?.response as Record<string, unknown> | undefined;
        const status: number = (axiosErr?.status as number) ?? 500;
        const data = axiosErr?.data as Record<string, unknown> | undefined;
        const message: string = (data?.error as string) ?? 'Failed to fetch market data';
        throw { status, message } as PulseApiError;
    }
}
