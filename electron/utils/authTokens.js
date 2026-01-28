"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const supabase_js_1 = require("@supabase/supabase-js");
const electron_1 = require("electron");
const path_1 = __importDefault(require("path"));
const fs_1 = __importDefault(require("fs"));
// Create Supabase client in Electron (reads from same storage as React)
async function getAuthToken() {
    try {
        const supabaseUrl = process.env.VITE_SUPABASE_URL;
        const supabaseAnonKey = process.env.VITE_SUPABASE_ANON_KEY;
        // Read session from Electron's localStorage equivalent
        // Supabase stores session in localStorage, which Electron can access
        if (!supabaseUrl || !supabaseAnonKey)
            return null;
        const supabase = (0, supabase_js_1.createClient)(supabaseUrl, supabaseAnonKey, {
            auth: {
                persistSession: true,
                storage: {
                    getItem: (key) => {
                        // Read from Electron's storage location
                        // This should match where React stores it
                        const storagePath = path_1.default.join(electron_1.app.getPath('userData'), 'supabase-session.json');
                        if (fs_1.default.existsSync(storagePath)) {
                            const data = JSON.parse(fs_1.default.readFileSync(storagePath, 'utf8'));
                            return data[key] || null;
                        }
                        return null;
                    },
                    setItem: (key, value) => {
                        // Write to Electron's storage
                        const storagePath = path_1.default.join(electron_1.app.getPath('userData'), 'supabase-session.json');
                        const data = fs_1.default.existsSync(storagePath)
                            ? JSON.parse(fs_1.default.readFileSync(storagePath, 'utf8'))
                            : {};
                        data[key] = value;
                        fs_1.default.writeFileSync(storagePath, JSON.stringify(data));
                    },
                    removeItem: (key) => {
                        const storagePath = path_1.default.join(electron_1.app.getPath('userData'), 'supabase-session.json');
                        if (fs_1.default.existsSync(storagePath)) {
                            const data = JSON.parse(fs_1.default.readFileSync(storagePath, 'utf8'));
                            delete data[key];
                            fs_1.default.writeFileSync(storagePath, JSON.stringify(data));
                        }
                    }
                }
            }
        });
        const { data: { session }, error } = await supabase.auth.getSession();
        if (error || !session?.access_token) {
            throw new Error('No active session found');
        }
        return session.access_token;
    }
    catch (error) {
        console.error('[getAuthToken] Error getting token:', error);
        throw error;
    }
}
module.exports = { getAuthToken };
//# sourceMappingURL=authTokens.js.map