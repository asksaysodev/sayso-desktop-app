import axios from 'axios';
import * as Sentry from "@sentry/electron/renderer";

const apiClient = axios.create({
	baseURL: import.meta.env.VITE_BACKEND_BASE_URL,
	timeout: 15000,
});

// ─── Request interceptor ─────────────────────────────────────────────────────
// Always fetch the token from main. main's AuthManager proactively refreshes
// 60 s before expiry, so this almost always returns a fresh token with zero
// round-trips. On the rare race where the token is stale, the 401 handler below
// forces a refresh and retries once.

apiClient.interceptors.request.use(
	async (config) => {
		const token: string | null = await window.electron?.ipcRenderer?.invoke('auth:get-token') ?? null;
		if (token) {
			config.headers.Authorization = `Bearer ${token}`;
		}

		Sentry.addBreadcrumb({
			category: 'api.request',
			message: `${config.method?.toUpperCase()} ${config.url}`,
			level: 'info',
			data: { method: config.method, url: config.url, baseURL: config.baseURL },
		});

		return config;
	},
	(error) => Promise.reject(error),
);

// ─── Response interceptor ────────────────────────────────────────────────────
// On 401: ask main to return the freshest token (which triggers a refresh if
// needed behind the mutex). Retry the original request exactly once.
// On network error: retry up to 3 times with back-off.

apiClient.interceptors.response.use(
	(response) => {
		Sentry.addBreadcrumb({
			category: 'api.response',
			message: `${response.config.method?.toUpperCase()} ${response.config.url} - ${response.status}`,
			level: 'info',
			data: { status: response.status, url: response.config.url },
		});
		return response;
	},
	async (error) => {
		const originalRequest = error.config;

		Sentry.addBreadcrumb({
			category: 'api.error',
			message: `${originalRequest?.method?.toUpperCase()} ${originalRequest?.url} - ${error.response?.status ?? error.code}`,
			level: 'error',
			data: {
				url: originalRequest?.url,
				status: error.response?.status,
				code: error.code,
				errorData: error.response?.data,
			},
		});

		// ── 401: force-refresh then retry once ─────────────────────────────
		if (error.response?.status === 401 && !originalRequest._retried) {
			originalRequest._retried = true;

			// Force an immediate refresh bypassing the 60-second proactive window.
			// Handles the case where the server rejects a token the client believed
			// was still valid (post-sleep, clock skew, key rotation, etc.).
			// forceRefresh() throws on transient errors (Wi-Fi reconnecting) so we
			// don't falsely treat a brief network blip as a permanent session expiry.
			let token: string | null = null;
			try {
				token = await window.electron?.ipcRenderer?.invoke('auth:force-refresh-token') ?? null;
			} catch {
				// Transient failure (Wi-Fi down, network blip) — don't dispatch session-expired
				return Promise.reject(error);
			}

			if (!token) {
				// null means invalid_grant — session is permanently gone
				window.dispatchEvent(new CustomEvent('auth:session-expired'));
				return Promise.reject(error);
			}

			originalRequest.headers['Authorization'] = `Bearer ${token}`;
			return apiClient(originalRequest);
		}

		// ── Network errors: up to 3 retries with back-off ───────────────────
		if (
			(error.code === 'ERR_NETWORK' || error.code === 'ERR_EMPTY_RESPONSE') &&
			originalRequest &&
			(originalRequest._retryCount ?? 0) < 3
		) {
			originalRequest._retryCount = (originalRequest._retryCount ?? 0) + 1;
			await new Promise(resolve => setTimeout(resolve, 1000 * originalRequest._retryCount));
			return apiClient(originalRequest);
		}

		return Promise.reject(error);
	},
);

// ─── Auth event listeners ────────────────────────────────────────────────────
// Main broadcasts these when auth state changes. The session-expired one is
// consumed by useSessionExpiry to close secondary windows gracefully.

window.electron?.ipcRenderer?.on('auth:session-expired', () => {
	window.dispatchEvent(new CustomEvent('auth:session-expired'));
});

// Backward-compat: old event name still dispatched by main during transition
window.electron?.ipcRenderer?.on('auth-session-expired', () => {
	window.dispatchEvent(new CustomEvent('auth:session-expired'));
});

export default apiClient;
