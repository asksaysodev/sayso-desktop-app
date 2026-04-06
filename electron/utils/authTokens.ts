import { app } from 'electron';
import path from 'path';
import fs from 'fs';

async function getAuthToken() {
  try {
    const storagePath = path.join(app.getPath('userData'), 'supabase-session.json');
    if (!fs.existsSync(storagePath)) return null;

    const data = JSON.parse(fs.readFileSync(storagePath, 'utf8'));
    const sessionKey = Object.keys(data).find(k => k.includes('auth'));
    if (!sessionKey) return null;

    const session = JSON.parse(data[sessionKey]);
    return session?.access_token ?? null;
  } catch (error) {
    console.error('[getAuthToken] Error getting token:', error);
    return null;
  }
}

async function refreshAuthTokens(currentRefreshToken: string): Promise<{ accessToken: string; refreshToken: string }> {
  const supabaseUrl = process.env.VITE_SUPABASE_URL;
  const supabaseAnonKey = process.env.VITE_SUPABASE_ANON_KEY;

  if (!supabaseUrl || !supabaseAnonKey) throw new Error('Missing Supabase config');

  const response = await fetch(`${supabaseUrl}/auth/v1/token?grant_type=refresh_token`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
      'apikey': supabaseAnonKey,
    },
    body: JSON.stringify({ refresh_token: currentRefreshToken }),
  });

  if (!response.ok) {
    const errorData = await response.json().catch(() => ({})) as { error_description?: string };
    throw new Error(errorData.error_description || `Token refresh failed: ${response.status}`);
  }

  const data = await response.json() as { access_token: string; refresh_token: string };
  return {
    accessToken: data.access_token,
    refreshToken: data.refresh_token,
  };
}

module.exports = { getAuthToken, refreshAuthTokens };
