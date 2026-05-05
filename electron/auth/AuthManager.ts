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

export class AuthManager extends EventEmitter {
  private accessToken: string | null = null;
  private refreshToken: string | null = null;
  private expiresAt: number | null = null;
  private user: AuthUser | null = null;
  private refreshTimer: NodeJS.Timeout | null = null;
  // Single in-flight refresh promise — all concurrent callers await the same one
  private refreshPromise: Promise<void> | null = null;

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
   * exchanges it for a fresh access token. If it fails, clears the stored token
   * so the user is prompted to sign in.
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
      console.warn('[AuthManager] Failed to restore session:', error.message);
      clearRefreshToken();
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
   */
  async getAccessToken(): Promise<string | null> {
    if (!this.refreshToken) return null;

    const now = Math.floor(Date.now() / 1000);
    const needsRefresh = !this.accessToken || !this.expiresAt || this.expiresAt - now < 60;

    if (needsRefresh) {
      await this._refresh();
    }

    return this.accessToken;
  }

  getState(): AuthState {
    return {
      user: this.user,
      isAuthenticated: !!(this.accessToken && this.user),
      accessToken: this.accessToken,
    };
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
      saveRefreshToken(this.refreshToken!);
      this._scheduleRefresh();
      this.emit('token-refreshed', this.getState());
      const remaining = this.expiresAt ? this.expiresAt - Math.floor(Date.now() / 1000) : 0;
      console.log(`[AuthManager] Token refreshed — next expiry in ${remaining}s`);
    } catch (error: any) {
      Sentry.captureException(error);
      console.error('[AuthManager] Refresh failed:', error.message);
      this._handleExpired();
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
        // _doRefresh already captures to Sentry and emits session-expired
      }
    }, delayMs);

    // Don't prevent the app from quitting just because the timer is pending
    this.refreshTimer.unref();
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
    Sentry.setUser(null);
    if (this.refreshTimer) {
      clearTimeout(this.refreshTimer);
      this.refreshTimer = null;
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

    const response = await fetch(`${this.supabaseUrl}${path}`, {
      method: 'POST',
      headers,
      body: JSON.stringify(body),
    });

    if (!response.ok) {
      const err = await response.json().catch(() => ({})) as {
        error_description?: string;
        message?: string;
        msg?: string;
      };
      throw new Error(err.error_description ?? err.message ?? err.msg ?? `Request failed: ${response.status}`);
    }

    return response.json() as Promise<T>;
  }

  private async _get<T>(path: string, accessToken: string): Promise<T> {
    const response = await fetch(`${this.supabaseUrl}${path}`, {
      headers: {
        'apikey': this.supabaseAnonKey,
        'Authorization': `Bearer ${accessToken}`,
      },
    });

    if (!response.ok) {
      const err = await response.json().catch(() => ({})) as {
        error_description?: string;
        message?: string;
      };
      throw new Error(err.error_description ?? err.message ?? `Request failed: ${response.status}`);
    }

    return response.json() as Promise<T>;
  }
}
