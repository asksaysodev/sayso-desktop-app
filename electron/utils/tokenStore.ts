import { safeStorage, app } from "electron/main";
import fs from "fs";
import path from "path";

const getAuthFilePath = () => path.join(app.getPath('userData'), 'auth.json');

export function saveRefreshToken(token: string) {
    if (!token) return;
    if (!safeStorage.isEncryptionAvailable()) return;

    const tokenBuffer = safeStorage.encryptString(token);
    const tokenToBase64 = tokenBuffer.toString('base64');
    fs.writeFileSync(getAuthFilePath(), JSON.stringify({ refreshToken: tokenToBase64 }));
}

export function loadRefreshToken(): string | null {
    try {
        const raw = fs.readFileSync(getAuthFilePath(), 'utf-8');
        const { refreshToken } = JSON.parse(raw);
        if (!refreshToken) return null;
        const buffer = Buffer.from(refreshToken, 'base64');
        return safeStorage.decryptString(buffer);
    } catch {
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
