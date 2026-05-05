"use strict";
var __createBinding = (this && this.__createBinding) || (Object.create ? (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    var desc = Object.getOwnPropertyDescriptor(m, k);
    if (!desc || ("get" in desc ? !m.__esModule : desc.writable || desc.configurable)) {
      desc = { enumerable: true, get: function() { return m[k]; } };
    }
    Object.defineProperty(o, k2, desc);
}) : (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    o[k2] = m[k];
}));
var __setModuleDefault = (this && this.__setModuleDefault) || (Object.create ? (function(o, v) {
    Object.defineProperty(o, "default", { enumerable: true, value: v });
}) : function(o, v) {
    o["default"] = v;
});
var __importStar = (this && this.__importStar) || (function () {
    var ownKeys = function(o) {
        ownKeys = Object.getOwnPropertyNames || function (o) {
            var ar = [];
            for (var k in o) if (Object.prototype.hasOwnProperty.call(o, k)) ar[ar.length] = k;
            return ar;
        };
        return ownKeys(o);
    };
    return function (mod) {
        if (mod && mod.__esModule) return mod;
        var result = {};
        if (mod != null) for (var k = ownKeys(mod), i = 0; i < k.length; i++) if (k[i] !== "default") __createBinding(result, mod, k[i]);
        __setModuleDefault(result, mod);
        return result;
    };
})();
Object.defineProperty(exports, "__esModule", { value: true });
exports.AuthManager = void 0;
const events_1 = require("events");
const Sentry = __importStar(require("@sentry/electron/main"));
const tokenStore_1 = require("../utils/tokenStore");
class AuthManager extends events_1.EventEmitter {
    accessToken = null;
    refreshToken = null;
    expiresAt = null;
    user = null;
    refreshTimer = null;
    // Single in-flight refresh promise — all concurrent callers await the same one
    refreshPromise = null;
    constructor() {
        super();
    }
    get supabaseUrl() {
        return (process.env.VITE_SUPABASE_URL ?? '').replace(/\/$/, '');
    }
    get supabaseAnonKey() {
        return process.env.VITE_SUPABASE_ANON_KEY ?? '';
    }
    // ─── Public API ────────────────────────────────────────────────────────────
    /**
     * Call once at app startup. Reads the persisted refresh token from disk and
     * exchanges it for a fresh access token. If it fails, clears the stored token
     * so the user is prompted to sign in.
     */
    async init() {
        const stored = (0, tokenStore_1.loadRefreshToken)();
        if (!stored) {
            console.log('[AuthManager] No stored refresh token — user needs to sign in');
            return;
        }
        try {
            await this._exchangeRefreshToken(stored);
            (0, tokenStore_1.saveRefreshToken)(this.refreshToken);
            this._scheduleRefresh();
            console.log('[AuthManager] Session restored for', this.user?.email);
        }
        catch (error) {
            console.warn('[AuthManager] Failed to restore session:', error.message);
            (0, tokenStore_1.clearRefreshToken)();
        }
    }
    /**
     * Signs in with email + password. Returns mfaRequired:true with the enrolled
     * factors if the account has TOTP set up. In that case the caller must follow
     * up with verifyMFA() before the session is considered fully authenticated.
     */
    async signIn(email, password) {
        try {
            const data = await this._post('/auth/v1/token?grant_type=password', { email, password });
            this._setTokens(data.access_token, data.refresh_token);
            // Check for enrolled TOTP factors to determine if MFA step is needed
            const factors = await this._getVerifiedFactors();
            const payload = this._decodePayload(data.access_token);
            const currentAAL = payload?.aal ?? 'aal1';
            if (factors.length > 0 && currentAAL === 'aal1') {
                // Keep tokens in memory but don't persist — MFA must complete first
                return { success: true, mfaRequired: true, mfaFactors: factors };
            }
            (0, tokenStore_1.saveRefreshToken)(this.refreshToken);
            this._scheduleRefresh();
            this.emit('signed-in', this.getState());
            return { success: true };
        }
        catch (error) {
            return { success: false, error: error.message };
        }
    }
    /**
     * Elevates an aal1 session to aal2 by verifying a TOTP code.
     * Must be called after signIn() when mfaRequired is true.
     */
    async verifyMFA(factorId, code) {
        if (!this.accessToken) {
            return { success: false, error: 'No active session to elevate' };
        }
        try {
            const challenge = await this._post(`/auth/v1/factors/${factorId}/challenge`, {}, this.accessToken);
            const result = await this._post(`/auth/v1/factors/${factorId}/verify`, { challenge_id: challenge.id, code }, this.accessToken);
            // MFA verify returns a new aal2 token pair — replaces the aal1 tokens
            this._setTokens(result.access_token, result.refresh_token);
            (0, tokenStore_1.saveRefreshToken)(this.refreshToken);
            this._scheduleRefresh();
            this.emit('signed-in', this.getState());
            return { success: true };
        }
        catch (error) {
            return { success: false, error: error.message };
        }
    }
    async signOut() {
        if (this.accessToken) {
            // Fire-and-forget — don't block local sign-out on network failure
            this._post('/auth/v1/logout', {}, this.accessToken).catch(() => { });
        }
        this._clearSession();
        (0, tokenStore_1.clearRefreshToken)();
        this.emit('signed-out');
    }
    /**
     * Returns the current access token, proactively refreshing if it is within
     * 60 seconds of expiry. All concurrent callers share one in-flight refresh.
     */
    async getAccessToken() {
        if (!this.refreshToken)
            return null;
        const now = Math.floor(Date.now() / 1000);
        const needsRefresh = !this.accessToken || !this.expiresAt || this.expiresAt - now < 60;
        if (needsRefresh) {
            await this._refresh();
        }
        return this.accessToken;
    }
    getState() {
        return {
            user: this.user,
            isAuthenticated: !!(this.accessToken && this.user),
            accessToken: this.accessToken,
        };
    }
    // ─── Private: refresh ──────────────────────────────────────────────────────
    async _refresh() {
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
    async _doRefresh() {
        try {
            await this._exchangeRefreshToken(this.refreshToken);
            (0, tokenStore_1.saveRefreshToken)(this.refreshToken);
            this._scheduleRefresh();
            this.emit('token-refreshed', this.getState());
            const remaining = this.expiresAt ? this.expiresAt - Math.floor(Date.now() / 1000) : 0;
            console.log(`[AuthManager] Token refreshed — next expiry in ${remaining}s`);
        }
        catch (error) {
            Sentry.captureException(error);
            console.error('[AuthManager] Refresh failed:', error.message);
            this._handleExpired();
            throw error;
        }
    }
    async _exchangeRefreshToken(token) {
        const data = await this._post('/auth/v1/token?grant_type=refresh_token', { refresh_token: token });
        this._setTokens(data.access_token, data.refresh_token);
    }
    _scheduleRefresh() {
        if (this.refreshTimer) {
            clearTimeout(this.refreshTimer);
            this.refreshTimer = null;
        }
        if (!this.expiresAt)
            return;
        const now = Math.floor(Date.now() / 1000);
        const delayMs = Math.max(0, (this.expiresAt - now - 60) * 1000);
        console.log(`[AuthManager] Next proactive refresh in ${Math.round(delayMs / 1000)}s`);
        this.refreshTimer = setTimeout(async () => {
            console.log('[AuthManager] Proactive refresh firing...');
            try {
                await this._refresh();
            }
            catch {
                // _doRefresh already captures to Sentry and emits session-expired
            }
        }, delayMs);
        // Don't prevent the app from quitting just because the timer is pending
        this.refreshTimer.unref();
    }
    // ─── Private: helpers ──────────────────────────────────────────────────────
    _setTokens(accessToken, refreshToken) {
        this.accessToken = accessToken;
        this.refreshToken = refreshToken;
        const payload = this._decodePayload(accessToken);
        this.expiresAt = payload?.exp ?? null;
        this.user = payload
            ? { id: payload.sub, email: payload.email ?? '', subscription_plan_id: null }
            : null;
    }
    _clearSession() {
        this.accessToken = null;
        this.refreshToken = null;
        this.expiresAt = null;
        this.user = null;
        if (this.refreshTimer) {
            clearTimeout(this.refreshTimer);
            this.refreshTimer = null;
        }
    }
    _handleExpired() {
        this._clearSession();
        (0, tokenStore_1.clearRefreshToken)();
        this.emit('session-expired');
    }
    _decodePayload(token) {
        try {
            return JSON.parse(Buffer.from(token.split('.')[1], 'base64').toString('utf-8'));
        }
        catch {
            return null;
        }
    }
    /**
     * Returns the user's verified TOTP factors by fetching /auth/v1/user.
     * Used after password sign-in to determine if an MFA step is required.
     */
    async _getVerifiedFactors() {
        if (!this.accessToken)
            return [];
        try {
            const user = await this._get('/auth/v1/user', this.accessToken);
            return (user.factors ?? []).filter(f => f.status === 'verified' && f.factor_type === 'totp');
        }
        catch {
            return [];
        }
    }
    // ─── Private: HTTP ─────────────────────────────────────────────────────────
    async _post(path, body, accessToken) {
        const headers = {
            'Content-Type': 'application/json',
            'apikey': this.supabaseAnonKey,
        };
        if (accessToken)
            headers['Authorization'] = `Bearer ${accessToken}`;
        const response = await fetch(`${this.supabaseUrl}${path}`, {
            method: 'POST',
            headers,
            body: JSON.stringify(body),
        });
        if (!response.ok) {
            const err = await response.json().catch(() => ({}));
            throw new Error(err.error_description ?? err.message ?? err.msg ?? `Request failed: ${response.status}`);
        }
        return response.json();
    }
    async _get(path, accessToken) {
        const response = await fetch(`${this.supabaseUrl}${path}`, {
            headers: {
                'apikey': this.supabaseAnonKey,
                'Authorization': `Bearer ${accessToken}`,
            },
        });
        if (!response.ok) {
            const err = await response.json().catch(() => ({}));
            throw new Error(err.error_description ?? err.message ?? `Request failed: ${response.status}`);
        }
        return response.json();
    }
}
exports.AuthManager = AuthManager;
//# sourceMappingURL=AuthManager.js.map