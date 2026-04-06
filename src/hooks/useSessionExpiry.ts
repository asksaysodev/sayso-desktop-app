import { useEffect } from 'react';

/**
 * Listens for the `auth:session-expired` event dispatched by the axios interceptor
 * when a token refresh fails (refresh token expired or revoked).
 * Secondary windows (coach, coach settings) use this to close themselves gracefully.
 */
export function useSessionExpiry(onExpired: () => void) {
    useEffect(() => {
        window.addEventListener('auth:session-expired', onExpired);
        return () => window.removeEventListener('auth:session-expired', onExpired);
    }, [onExpired]);
}
