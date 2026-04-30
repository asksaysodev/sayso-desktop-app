import { useEffect, useRef, useState } from 'react';
import { useMutation } from '@tanstack/react-query';
import getPulseMarketProperty, { MarketProperty, PulseApiError } from '../services/getPulseMarketProperty';

export default function usePulseMarketProperty(zipCodeValue: string) {
    const [selectedPropertyType, setSelectedPropertyType] = useState('');
    const [valuesFound, setValuesFound] = useState<MarketProperty | null>(null);
    const [pulseError, setPulseError] = useState<PulseApiError | null>(null);
    // Track the zip for which a fetch was issued so stale responses are discarded.
    const pendingZipRef = useRef<string>('');

    useEffect(() => {
        setValuesFound(null);
        setPulseError(null);
        setSelectedPropertyType('');
    }, [zipCodeValue]);

    const { mutate, isPending } = useMutation({
        mutationFn: () => {
            pendingZipRef.current = zipCodeValue;
            return getPulseMarketProperty(zipCodeValue, selectedPropertyType);
        },
        onSuccess: (data) => {
            if (pendingZipRef.current !== zipCodeValue) return;
            setValuesFound(data);
            setPulseError(null);
        },
        onError: (err: PulseApiError) => {
            if (pendingZipRef.current !== zipCodeValue) return;
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
