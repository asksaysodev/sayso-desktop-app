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

export default async function getPulseMarketProperty(zipCode: string, propertyType: string): Promise<MarketProperty> {
    try {
        const response = await apiClient.get(`/pulse/market?zipCode=${zipCode}&propertyType=${propertyType}`);

        if (!response?.data) {
            throw { status: 500, message: 'Failed to fetch market data' } as PulseApiError;
        }

        return response.data;
    } catch (err: any) {
        if (err?.status && err?.message) throw err;
        const status: number = err?.response?.status ?? 500;
        const message: string = err?.response?.data?.error ?? 'Failed to fetch market data';
        throw { status, message } as PulseApiError;
    }
}
