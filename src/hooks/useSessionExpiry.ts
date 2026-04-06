import { useEffect, useLayoutEffect, useRef } from 'react';

/**
 * Listens for the `auth:session-expired` event dispatched by the axios interceptor
 * when a token refresh fails (refresh token expired or revoked).
 * Secondary windows (coach, coach settings) use this to close themselves gracefully.
 *
 * Uses a ref to stabilize the callback so the event listener is registered once,
 * regardless of whether the caller wrapped the callback in useCallback or not.
 */
export function useSessionExpiry(onExpired: () => void) {
    const onExpiredRef = useRef(onExpired);
    useLayoutEffect(() => { onExpiredRef.current = onExpired; });

    useEffect(() => {
        const handler = () => onExpiredRef.current();
        window.addEventListener('auth:session-expired', handler);
        return () => window.removeEventListener('auth:session-expired', handler);
    }, []);
}
