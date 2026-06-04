import { EventEmitter } from 'events';
import * as Sentry from '@sentry/electron/main';
import { loadRefreshToken, saveRefreshToken, clearRefreshToken } from '../utils/tokenStore';
import type { AuthUser } from '../globals';

interface JwtPayload {
  sub: string;
  email?: string;
  exp?: number;
  aal?: string;
  amr?: Array<{ method: string; timestamp: number }>;
}

export interface MFAFactor {
  id: string;
  factor_type: string;
  status: string;
}

export interface AuthState {
  user: AuthUser | null;
  isAuthenticated: boolean;
  accessToken: string | null;
}

export interface SignInResult {
  success: boolean;
  mfaRequired?: boolean;
  mfaFactors?: MFAFactor[];
  error?: string;
}

// Supabase error codes that mean the refresh token is permanently invalid.
// Any other failure (network, 5xx, unknown) is treated as transient.
const TERMINAL_GRANT_ERRORS = new Set([
  'invalid_grant',
  'refresh_token_not_found',
  'refresh_token_already_used',
  'user_not_found',
  'user_banned',
]);

class AuthError extends Error {
  constructor(
    public readonly kind: 'network' | 'invalid_grant' | 'unknown',
    message: string,
    public readonly status?: number,
  ) {
    super(message);
    this.name = 'AuthError';
  }
}

// Backoff delays (ms) for transient refresh failures: 1s, 3s, 10s, 30s, 60s cap
const RETRY_DELAYS_MS = [1_000, 3_000, 10_000, 30_000, 60_000];

export class AuthManager extends EventEmitter {
  private accessToken: string | null = null;
  private refreshToken: string | null = null;
  private expiresAt: number | null = null;
  private user: AuthUser | null = null;
  private refreshTimer: NodeJS.Timeout | null = null;
  // Single in-flight refresh promise — all concurrent callers await the same one
  private refreshPromise: Promise<void> | null = null;
  // Network-error retry state
  private networkRetryCount = 0;
  private networkRetryTimer: NodeJS.Timeout | null = null;
  // True while the OS reports us offline. Blocks all auto-scheduled refresh work
  // (proactive expiry timer, transient retry backoff) so we don't generate Sentry
  // noise during an extended outage. Cleared by forceRefresh() / signIn().
  private paused = false;

  constructor() {
    super();
  }

  private get supabaseUrl(): string {
    return (process.env.VITE_SUPABASE_URL ?? '').replace(/\/$/, '');
  }

  private get supabaseAnonKey(): string {
    return process.env.VITE_SUPABASE_ANON_KEY ?? '';
  }

  // ─── Public API ────────────────────────────────────────────────────────────

  /**
   * Call once at app startup. Reads the persisted refresh token from disk and
   * exchanges it for a fresh access token.
   *
   * On network failure we keep the token on disk and schedule a retry — the user
   * should NOT be logged out just because Wi-Fi wasn't ready at boot time.
   * Only a definitive Supabase auth error (invalid_grant etc.) clears the token.
   */
  async init(): Promise<void> {
    const stored = loadRefreshToken();
    if (!stored) {
      console.log('[AuthManager] No stored refresh token — user needs to sign in');
      return;
    }

    try {
      await this._exchangeRefreshToken(stored);
      saveRefreshToken(this.refreshToken!);
      this._scheduleRefresh();
      console.log('[AuthManager] Session restored for', this.user?.email);
    } catch (error: any) {
      if (error instanceof AuthError && error.kind === 'invalid_grant') {
        // Refresh token is permanently invalid — clear it so the user signs in fresh
        console.warn('[AuthManager] Refresh token invalid at init — clearing session');
        clearRefreshToken();
      } else {
        // Transient failure (offline at boot, slow Wi-Fi, etc.) — keep the token
        // on disk and in memory; a retry will pick it up when the network is ready
        console.warn('[AuthManager] Failed to restore session (transient), will retry:', error.message);
        this.refreshToken = stored;
        this._scheduleNetworkRetry();
      }
    }
  }

  /**
   * Signs in with email + password. Returns mfaRequired:true with the enrolled
   * factors if the account has TOTP set up. In that case the caller must follow
   * up with verifyMFA() before the session is considered fully authenticated.
   */
  async signIn(email: string, password: string): Promise<SignInResult> {
    try {
      const data = await this._post<{ access_token: string; refresh_token: string }>(
        '/auth/v1/token?grant_type=password',
        { email, password },
      );

      this._setTokens(data.access_token, data.refresh_token);

      // Check for enrolled TOTP factors to determine if MFA step is needed
      const factors = await this._getVerifiedFactors();
      const payload = this._decodePayload(data.access_token);
      const currentAAL = payload?.aal ?? 'aal1';

      if (factors.length > 0 && currentAAL === 'aal1') {
        // Keep tokens in memory but don't persist — MFA must complete first
        return { success: true, mfaRequired: true, mfaFactors: factors };
      }

      saveRefreshToken(this.refreshToken!);
      this._scheduleRefresh();
      this.emit('signed-in', this.getState());
      return { success: true };
    } catch (error: any) {
      return { success: false, error: error.message };
    }
  }

  /**
   * Elevates an aal1 session to aal2 by verifying a TOTP code.
   * Must be called after signIn() when mfaRequired is true.
   */
  async verifyMFA(factorId: string, code: string): Promise<{ success: boolean; error?: string }> {
    if (!this.accessToken) {
      return { success: false, error: 'No active session to elevate' };
    }

    try {
      const challenge = await this._post<{ id: string }>(
        `/auth/v1/factors/${factorId}/challenge`,
        {},
        this.accessToken,
      );

      const result = await this._post<{ access_token: string; refresh_token: string }>(
        `/auth/v1/factors/${factorId}/verify`,
        { challenge_id: challenge.id, code },
        this.accessToken,
      );

      // MFA verify returns a new aal2 token pair — replaces the aal1 tokens
      this._setTokens(result.access_token, result.refresh_token);
      saveRefreshToken(this.refreshToken!);
      this._scheduleRefresh();
      this.emit('signed-in', this.getState());
      return { success: true };
    } catch (error: any) {
      return { success: false, error: error.message };
    }
  }

  async signOut(): Promise<void> {
    if (this.accessToken) {
      // Fire-and-forget — don't block local sign-out on network failure
      this._post('/auth/v1/logout', {}, this.accessToken).catch(() => {});
    }
    this._clearSession();
    clearRefreshToken();
    this.emit('signed-out');
  }

  /**
   * Returns the current access token, proactively refreshing if it is within
   * 60 seconds of expiry. All concurrent callers share one in-flight refresh.
   *
   * On a transient network failure this returns whatever token we have (possibly
   * stale) so the caller can still attempt the request — the axios 401 handler
   * will call forceRefresh() which retries with proper backoff.
   */
  async getAccessToken(): Promise<string | null> {
    if (!this.refreshToken) return null;

    const now = Math.floor(Date.now() / 1000);
    const needsRefresh = !this.accessToken || !this.expiresAt || this.expiresAt - now < 60;

    if (needsRefresh) {
      try {
        await this._refresh();
      } catch (error: any) {
        if (error instanceof AuthError && error.kind === 'invalid_grant') {
          return null; // session is definitively gone
        }
        // Transient failure — return whatever token we have; the response
        // interceptor's force-refresh path will handle the resulting 401
      }
    }

    return this.accessToken;
  }

  /**
   * Forces an immediate token refresh regardless of expiry, bypassing the
   * 60-second proactive window. Used by the powerMonitor resume/unlock handlers
   * and by the axios 401 interceptor when the server rejects a token the client
   * believed was still valid (clock skew, key rotation, etc.).
   *
   * Returns the new access token, or null if the session is expired.
   */
  async forceRefresh(): Promise<string | null> {
    if (!this.refreshToken) return null; // session truly gone
    // Caller (OS online event, powerMonitor, axios 401 retry) explicitly believes
    // network is available — lift the offline pause so a transient failure here
    // can still schedule its own backoff.
    this.paused = false;
    try {
      await this._refresh();
      return this.accessToken;
    } catch (error: any) {
      if (error instanceof AuthError && error.kind === 'invalid_grant') {
        return null; // session permanently gone
      }
      throw error; // transient — propagate so caller doesn't treat as session-expired
    }
  }

  getState(): AuthState {
    return {
      user: this.user,
      isAuthenticated: !!(this.accessToken && this.user),
      accessToken: this.accessToken,
    };
  }

  isNetworkRetryPending(): boolean {
    return this.networkRetryTimer !== null;
  }

  /**
   * Suspends all auto-scheduled refresh work. Call this when the OS signals
   * we're offline. Clears both the proactive expiry timer and any pending
   * transient-retry timer, and blocks future auto-schedules until forceRefresh()
   * (typically driven by the OS 'online' event) flips paused back off.
   *
   * The session itself is preserved — tokens stay in memory and on disk.
   */
  pauseRefresh(): void {
    if (!this.paused) {
      this.paused = true;
      console.log('[AuthManager] Refresh paused (OS offline)');
    }
    if (this.refreshTimer) {
      clearTimeout(this.refreshTimer);
      this.refreshTimer = null;
    }
    if (this.networkRetryTimer) {
      clearTimeout(this.networkRetryTimer);
      this.networkRetryTimer = null;
    }
  }

  // ─── Private: refresh ──────────────────────────────────────────────────────

  private async _refresh(): Promise<void> {
    // If a refresh is already in flight, await it instead of starting a second one
    if (this.refreshPromise) {
      await this.refreshPromise;
      return;
    }

    if (!this.refreshToken) {
      this._handleExpired();
      return;
    }

    this.refreshPromise = this._doRefresh().finally(() => {
      this.refreshPromise = null;
    });

    await this.refreshPromise;
  }

  private async _doRefresh(): Promise<void> {
    try {
      await this._exchangeRefreshToken(this.refreshToken!);
      // Success — reset retry state
      this.networkRetryCount = 0;
      if (this.networkRetryTimer) {
        clearTimeout(this.networkRetryTimer);
        this.networkRetryTimer = null;
      }
      saveRefreshToken(this.refreshToken!);
      this._scheduleRefresh();
      this.emit('token-refreshed', this.getState());
      const remaining = this.expiresAt ? this.expiresAt - Math.floor(Date.now() / 1000) : 0;
      console.log(`[AuthManager] Token refreshed — next expiry in ${remaining}s`);
    } catch (error: any) {
      Sentry.captureException(error);
      if (error instanceof AuthError && error.kind === 'invalid_grant') {
        // Refresh token is permanently invalid — treat as a true logout
        console.error('[AuthManager] Refresh token rejected by server — ending session');
        this._handleExpired();
      } else {
        // Transient failure (network down, 5xx, etc.) — keep the session alive
        // and schedule a retry so the user isn't logged out by a flaky network
        console.warn('[AuthManager] Refresh failed (transient), will retry:', error.message);
        this._scheduleNetworkRetry();
      }
      throw error;
    }
  }

  private async _exchangeRefreshToken(token: string): Promise<void> {
    const data = await this._post<{ access_token: string; refresh_token: string }>(
      '/auth/v1/token?grant_type=refresh_token',
      { refresh_token: token },
    );
    this._setTokens(data.access_token, data.refresh_token);
  }

  private _scheduleRefresh(): void {
    if (this.refreshTimer) {
      clearTimeout(this.refreshTimer);
      this.refreshTimer = null;
    }

    if (!this.expiresAt) return;

    const now = Math.floor(Date.now() / 1000);
    const delayMs = Math.max(0, (this.expiresAt - now - 60) * 1000);

    console.log(`[AuthManager] Next proactive refresh in ${Math.round(delayMs / 1000)}s`);

    this.refreshTimer = setTimeout(async () => {
      console.log('[AuthManager] Proactive refresh firing...');
      try {
        await this._refresh();
      } catch {
        // _doRefresh already captures to Sentry and schedules a network retry
        // if transient, or calls _handleExpired if invalid_grant
      }
    }, delayMs);

    // Don't prevent the app from quitting just because the timer is pending
    this.refreshTimer.unref();
  }

  /**
   * Schedules a retry after a transient refresh failure using exponential backoff.
   * Does nothing if a retry is already pending.
   */
  private _scheduleNetworkRetry(): void {
    if (this.paused) return; // OS says we're offline — pointless to keep trying
    if (this.networkRetryTimer) return; // already pending

    const attemptNumber = this.networkRetryCount + 1;
    const delay = RETRY_DELAYS_MS[Math.min(this.networkRetryCount, RETRY_DELAYS_MS.length - 1)];

    console.log(`[AuthManager] Scheduling refresh retry #${attemptNumber} in ${delay}ms`);

    this.networkRetryTimer = setTimeout(async () => {
      this.networkRetryTimer = null;
      this.networkRetryCount = attemptNumber;
      if (!this.refreshToken) return; // session was explicitly cleared
      console.log(`[AuthManager] Retry #${attemptNumber} firing...`);
      try {
        await this._refresh();
      } catch {
        // _doRefresh will reschedule if still transient, or end session if invalid_grant
      }
    }, delay);

    this.networkRetryTimer.unref();
  }

  // ─── Private: helpers ──────────────────────────────────────────────────────

  private _setTokens(accessToken: string, refreshToken: string): void {
    this.accessToken = accessToken;
    this.refreshToken = refreshToken;
    const payload = this._decodePayload(accessToken);
    this.expiresAt = payload?.exp ?? null;
    this.user = payload
      ? { id: payload.sub, email: payload.email ?? '', subscription_plan_id: null }
      : null;
    if (this.user) {
      Sentry.setUser({ id: this.user.id, email: this.user.email });
    }
  }

  private _clearSession(): void {
    this.accessToken = null;
    this.refreshToken = null;
    this.expiresAt = null;
    this.user = null;
    this.networkRetryCount = 0;
    this.paused = false;
    Sentry.setUser(null);
    if (this.refreshTimer) {
      clearTimeout(this.refreshTimer);
      this.refreshTimer = null;
    }
    if (this.networkRetryTimer) {
      clearTimeout(this.networkRetryTimer);
      this.networkRetryTimer = null;
    }
  }

  private _handleExpired(): void {
    this._clearSession();
    clearRefreshToken();
    this.emit('session-expired');
  }

  private _decodePayload(token: string): JwtPayload | null {
    try {
      return JSON.parse(Buffer.from(token.split('.')[1], 'base64url').toString('utf-8')) as JwtPayload;
    } catch {
      return null;
    }
  }

  /**
   * Returns the user's verified TOTP factors by fetching /auth/v1/user.
   * Used after password sign-in to determine if an MFA step is required.
   */
  private async _getVerifiedFactors(): Promise<MFAFactor[]> {
    if (!this.accessToken) return [];
    try {
      const user = await this._get<{ factors?: MFAFactor[] }>('/auth/v1/user', this.accessToken);
      return (user.factors ?? []).filter(f => f.status === 'verified' && f.factor_type === 'totp');
    } catch {
      return [];
    }
  }

  // ─── Private: HTTP ─────────────────────────────────────────────────────────

  private async _post<T>(path: string, body: object, accessToken?: string): Promise<T> {
    const headers: Record<string, string> = {
      'Content-Type': 'application/json',
      'apikey': this.supabaseAnonKey,
    };
    if (accessToken) headers['Authorization'] = `Bearer ${accessToken}`;

    let response: Response;
    try {
      response = await fetch(`${this.supabaseUrl}${path}`, {
        method: 'POST',
        headers,
        body: JSON.stringify(body),
      });
    } catch (fetchError: any) {
      // fetch() itself threw — DNS failure, ENOTFOUND, offline, timeout, etc.
      throw new AuthError('network', fetchError.message ?? 'Network error');
    }

    if (!response.ok) {
      const err = await response.json().catch(() => ({})) as {
        error?: string;
        error_code?: string;
        error_description?: string;
        message?: string;
        msg?: string;
      };

      const errorCode = err.error_code ?? err.error ?? '';
      const errorMessage = err.error_description ?? err.message ?? err.msg ?? err.error;

      if (TERMINAL_GRANT_ERRORS.has(errorCode)) {
        throw new AuthError(
          'invalid_grant',
          errorMessage ?? `Auth rejected: ${response.status}`,
          response.status,
        );
      }

      // 5xx or unrecognised 4xx — treat as transient
      throw new AuthError(
        'unknown',
        errorMessage ?? `Request failed: ${response.status}`,
        response.status,
      );
    }

    return response.json() as Promise<T>;
  }

  private async _get<T>(path: string, accessToken: string): Promise<T> {
    let response: Response;
    try {
      response = await fetch(`${this.supabaseUrl}${path}`, {
        headers: {
          'apikey': this.supabaseAnonKey,
          'Authorization': `Bearer ${accessToken}`,
        },
      });
    } catch (fetchError: any) {
      throw new AuthError('network', fetchError.message ?? 'Network error');
    }

    if (!response.ok) {
      const err = await response.json().catch(() => ({})) as {
        error?: string;
        error_code?: string;
        error_description?: string;
        message?: string;
        msg?: string;
      };

      const errorCode = err.error_code ?? err.error ?? '';
      const errorMessage = err.error_description ?? err.message ?? err.msg ?? err.error;

      if (TERMINAL_GRANT_ERRORS.has(errorCode)) {
        throw new AuthError(
          'invalid_grant',
          errorMessage ?? `Auth rejected: ${response.status}`,
          response.status,
        );
      }
      throw new AuthError(
        'unknown',
        errorMessage ?? `Request failed: ${response.status}`,
        response.status,
      );
    }

    return response.json() as Promise<T>;
  }
}
