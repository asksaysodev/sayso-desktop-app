import { safeStorage, app } from "electron/main";
import fs from "fs";
import path from "path";

const getAuthFilePath = () => path.join(app.getPath('userData'), 'auth.json');

export function saveRefreshToken(token: string) {
    if (!token) return;
    if (!safeStorage.isEncryptionAvailable()) {
        console.error('[tokenStore] safeStorage encryption unavailable — refresh token not saved');
        return;
    }
    try {
        const tokenBuffer = safeStorage.encryptString(token);
        const tokenToBase64 = tokenBuffer.toString('base64');
        fs.writeFileSync(getAuthFilePath(), JSON.stringify({ refreshToken: tokenToBase64 }));
    } catch (err) {
        console.error('[tokenStore] Failed to save refresh token:', err);
    }
}

export function loadRefreshToken(): string | null {
    if (!safeStorage.isEncryptionAvailable()) {
        console.error('[tokenStore] safeStorage encryption unavailable — cannot load refresh token');
        return null;
    }
    try {
        const raw = fs.readFileSync(getAuthFilePath(), 'utf-8');
        const { refreshToken } = JSON.parse(raw);
        if (!refreshToken) return null;
        const buffer = Buffer.from(refreshToken, 'base64');
        return safeStorage.decryptString(buffer);
    } catch (err) {
        if ((err as NodeJS.ErrnoException).code !== 'ENOENT') {
            console.error('[tokenStore] Failed to load refresh token:', err);
        }
        return null;
    }
}

export function clearRefreshToken() {
    try {
        fs.unlinkSync(getAuthFilePath());
    } catch {
        // file didn't exist, nothing to clear
    }
}
