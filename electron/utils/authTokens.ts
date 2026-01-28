import { createClient } from '@supabase/supabase-js';
import { app } from 'electron';
import path from 'path';
import fs from 'fs';

// Create Supabase client in Electron (reads from same storage as React)
async function getAuthToken() {
  try {
    const supabaseUrl = process.env.VITE_SUPABASE_URL;
    const supabaseAnonKey = process.env.VITE_SUPABASE_ANON_KEY;
    
    // Read session from Electron's localStorage equivalent
    // Supabase stores session in localStorage, which Electron can access
    if (!supabaseUrl || !supabaseAnonKey) return null;

    const supabase = createClient(supabaseUrl, supabaseAnonKey, {
      auth: {
        persistSession: true,
        storage: {
          getItem: (key: string) => {
            // Read from Electron's storage location
            // This should match where React stores it
            const storagePath = path.join(app.getPath('userData'), 'supabase-session.json');
            if (fs.existsSync(storagePath)) {
              const data = JSON.parse(fs.readFileSync(storagePath, 'utf8'));
              return data[key] || null;
            }
            return null;
          },
          setItem: (key: string, value: string) => {
            // Write to Electron's storage
            const storagePath = path.join(app.getPath('userData'), 'supabase-session.json');
            const data = fs.existsSync(storagePath) 
              ? JSON.parse(fs.readFileSync(storagePath, 'utf8'))
              : {};
            data[key] = value;
            fs.writeFileSync(storagePath, JSON.stringify(data));
          },
          removeItem: (key: string) => {
            const storagePath = path.join(app.getPath('userData'), 'supabase-session.json');
            if (fs.existsSync(storagePath)) {
              const data = JSON.parse(fs.readFileSync(storagePath, 'utf8'));
              delete data[key];
              fs.writeFileSync(storagePath, JSON.stringify(data));
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
  } catch (error) {
    console.error('[getAuthToken] Error getting token:', error);
    throw error;
  }
}

module.exports = { getAuthToken };