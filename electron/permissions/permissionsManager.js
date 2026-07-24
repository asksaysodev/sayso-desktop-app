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
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.isPermissionsComplete = isPermissionsComplete;
exports.checkOSPermissionsGranted = checkOSPermissionsGranted;
exports.registerPermissionsIpc = registerPermissionsIpc;
const electron_1 = require("electron");
const Sentry = __importStar(require("@sentry/electron/main"));
const index_1 = __importDefault(require("./index"));
// app.isPackaged is reliable at module-load time; NODE_ENV is not yet set when
// this module is imported. (Same reasoning as audioManager.)
const isDev = !electron_1.app.isPackaged;
/**
 * Are all OS permissions required to run granted (and onboarding flag set)?
 * Resilient wrapper for startup routing — never throws (returns false on error).
 */
function isPermissionsComplete() {
    try {
        return index_1.default.isComplete();
    }
    catch {
        return false;
    }
}
/** Live mic + screen grant status. Used by the cue pre-flight and permission polling. */
function checkOSPermissionsGranted() {
    return index_1.default.checkGranted();
}
/** Register the permissions-* IPC handlers. Call once during app init. */
function registerPermissionsIpc() {
    // Check current mic + screen status (non-interactive)
    electron_1.ipcMain.handle('permissions-check', async () => {
        try {
            const result = await index_1.default.checkGranted();
            if (isDev)
                console.log('[Permissions] check:', result);
            return { mic: result.mic, screen: result.screen };
        }
        catch (e) {
            console.error('[MAIN] [Permissions] Error checking permissions:', e);
            Sentry.captureException(e);
            return { mic: false, screen: false, error: e.message };
        }
    });
    // Request microphone permission only (never triggers app restart)
    electron_1.ipcMain.handle('permissions-request-mic', async () => {
        try {
            return await index_1.default.requestMic();
        }
        catch (e) {
            console.error('[MAIN] [Permissions] Error requesting mic:', e);
            Sentry.captureException(e);
            return { mic: false, action: 'error', error: e.message };
        }
    });
    // Non-destructive poll: did the user grant screen recording?
    electron_1.ipcMain.handle('permissions-check-screen', async () => {
        try {
            const result = await index_1.default.checkGranted();
            return result.screen;
        }
        catch {
            return false;
        }
    });
    // Prompt macOS to surface the Screen Recording row in System Settings (fire-and-forget)
    electron_1.ipcMain.handle('permissions-request-screen', () => {
        index_1.default.requestScreen();
    });
    // Open Screen Recording privacy pane directly
    electron_1.ipcMain.handle('permissions-open-screen-settings', async () => {
        try {
            await index_1.default.openScreenSettings();
        }
        catch (e) {
            console.warn('[MAIN] [Permissions] Could not open Screen Recording settings:', e?.message || e);
        }
    });
    // Write permissions-complete flag then relaunch. Only relaunch if the write succeeded —
    // otherwise the next boot would route back to permissions (potential loop).
    electron_1.ipcMain.handle('permissions-complete', () => {
        try {
            index_1.default.markComplete();
            console.log('[Permissions] permissions-complete flag written');
        }
        catch (e) {
            console.error('[MAIN] [Permissions] Failed to write permissions-complete flag:', e);
            Sentry.captureException(e);
            return { error: e.message };
        }
        electron_1.app.relaunch();
        electron_1.app.quit();
    });
    // Returns whether the permissions-complete flag is set (for renderer routing)
    electron_1.ipcMain.handle('permissions-get-flag', () => isPermissionsComplete());
}
//# sourceMappingURL=permissionsManager.js.map