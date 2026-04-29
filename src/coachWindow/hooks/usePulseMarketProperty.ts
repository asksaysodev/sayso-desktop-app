import { useEffect, useState } from 'react';
import { useMutation } from '@tanstack/react-query';
import getPulseMarketProperty, { MarketProperty, PulseApiError } from '../services/getPulseMarketProperty';

export default function usePulseMarketProperty(zipCodeValue: string) {
    const [selectedPropertyType, setSelectedPropertyType] = useState('');
    const [valuesFound, setValuesFound] = useState<MarketProperty | null>(null);
    const [pulseError, setPulseError] = useState<PulseApiError | null>(null);

    useEffect(() => {
        setValuesFound(null);
        setPulseError(null);
        setSelectedPropertyType('');
    }, [zipCodeValue]);

    const { mutate, isPending } = useMutation({
        mutationFn: () => getPulseMarketProperty(zipCodeValue, selectedPropertyType),
        mutationKey: ['get-pulse-market-property'],
        onSuccess: (data) => {
            setValuesFound(data);
            setPulseError(null);
        },
        onError: (err: PulseApiError) => {
            setPulseError(err);
        },
    });

    return {
        selectedPropertyType,
        setSelectedPropertyType,
        valuesFound,
        pulseError,
        isPending,
        fetch: mutate,
    };
}
