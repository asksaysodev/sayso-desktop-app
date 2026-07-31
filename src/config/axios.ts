import axios from 'axios';
import * as Sentry from "@sentry/electron/renderer";

const apiClient = axios.create({
	baseURL: import.meta.env.VITE_BACKEND_BASE_URL,
	timeout: 15000,
});

// ─── Network state mirror ────────────────────────────────────────────────────
// Kept as a module-level cache rather than an IPC round-trip, because it's read
// from inside the failure path of a request that has already timed out. Main
// only ever reports these two values (electron/main.ts, network:report-status).

let networkState: 'online' | 'reconnecting' = 'online';
const setNetworkState = (state: unknown) => {
	if (state === 'online' || state === 'reconnecting') networkState = state;
};
window.electron?.ipcRenderer?.invoke('network:get-state').then(setNetworkState).catch(() => {});
window.electron?.ipcRenderer?.on('network:state-changed', setNetworkState);

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
			// The handler returns a discriminated result — never throws across IPC —
			// so we don't depend on error serialisation preserving typed fields.
			type ForceRefreshResult =
				| { ok: true; token: string }
				| { ok: false; kind: 'invalid_grant' | 'transient' };

			const result = (await window.electron?.ipcRenderer?.invoke(
				'auth:force-refresh-token',
			)) as ForceRefreshResult | undefined;

			if (!result || !result.ok) {
				if (result?.kind === 'invalid_grant') {
					// Session is permanently gone — let useSessionExpiry react
					window.dispatchEvent(new CustomEvent('auth:session-expired'));
				}
				// transient failure OR no electron context → silently reject without
				// firing session-expired (Wi-Fi blip should not log the user out)
				return Promise.reject(error);
			}

			originalRequest.headers['Authorization'] = `Bearer ${result.token}`;
			return apiClient(originalRequest);
		}

		// ── Network errors & timeouts: up to 3 retries with back-off ────────
		// ECONNABORTED is what axios reports on a request timeout ("timeout of
		// Nms exceeded"). Treat it like a transient network blip and retry.
		if (
			(error.code === 'ERR_NETWORK' ||
				error.code === 'ERR_EMPTY_RESPONSE' ||
				error.code === 'ECONNABORTED') &&
			originalRequest &&
			(originalRequest._retryCount ?? 0) < 3
		) {
			// Once the app knows it's offline, further backoff rounds only keep the
			// user on a spinner for a result we can already predict. Fail fast — the
			// offline banner explains the state better than a delayed error does.
			// Checked after the eligibility test so every attempt still leaves a
			// breadcrumb above. SAYSO-336.
			if (networkState === 'reconnecting') return Promise.reject(error);

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
