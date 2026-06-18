import { useEffect, useRef, useState } from 'react';
import { useMutation } from '@tanstack/react-query';
import getPulseMarketProperty, { PulseResponse, PulseApiError } from '../services/getPulseMarketProperty';

export default function usePulseMarketProperty(zipCodeValue: string, sessionId: string) {
	const MAX_ZIP_CACHE = 20;

	function setCached(cache: Map<string, PulseResponse>, key: string, value: PulseResponse) {
		if(cache.size >= MAX_ZIP_CACHE) {
			cache.delete(cache.keys().next().value!);
		}
		cache.set(key, value);
	}
    const [allResults, setAllResults] = useState<PulseResponse | null>(null);
    const [pulseError, setPulseError] = useState<PulseApiError | null>(null);
    const pendingZipRef = useRef<string>('');
    const cacheRef = useRef<Map<string, PulseResponse>>(new Map());

    const { mutate, isPending } = useMutation({
        mutationFn: () => {
            pendingZipRef.current = zipCodeValue;
            return getPulseMarketProperty(zipCodeValue, sessionId);
        },
        onMutate: () => {
            setAllResults(null);
            setPulseError(null);
        },
        onSuccess: (data) => {
            if (pendingZipRef.current !== zipCodeValue) return;
            setCached(cacheRef.current, zipCodeValue, data);
            setAllResults(data);
        },
        onError: (err: PulseApiError) => {
            if (pendingZipRef.current !== zipCodeValue) return;
            setPulseError(err);
        },
    });

    useEffect(() => {
        if (zipCodeValue.length !== 5) {
            setAllResults(null);
            setPulseError(null);
            return;
        }
        const cached = cacheRef.current.get(zipCodeValue);
        if (cached) {
            setAllResults(cached);
            setPulseError(null);
            return;
        }
        mutate();
    }, [zipCodeValue, mutate]);

    return {
        allResults,
        pulseError,
        isPending,
        retry: mutate,
    };
}
