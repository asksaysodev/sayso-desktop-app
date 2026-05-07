import apiClient from "@/config/axios";

export interface MarketProperty {
    property: {
        location: {
            zipCode: string;
            city: string;
            state: string;
        },
        type: string;
    },
    data: {
        key: string;
        value: string;
        label: string;
        display: boolean;
        trend?: {
            direction: "up" | "down" | "neutral";
            changePercent: number;
        }
    }[]
}

export interface PulseApiError {
    status: number;
    message: string;
}

export default async function getPulseMarketProperty(zipCode: string, propertyType: string, sessionId: string): Promise<MarketProperty> {
    try {
        const response = await apiClient.post('/pulse/market', { zipCode, propertyType, sessionId });

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
