import { supabase } from '../config/supabase';

// Access environment variables provided by Vite/Electron build process
// Note: Vite automatically loads .env variables and makes them available 
// via import.meta.env prefixed with VITE_
// For Electron's main process, we might need `dotenv` later if accessing there.
// For the renderer (React app), Vite handles it.

// We need to prefix them with VITE_ for Vite to expose them to the client-side code
const supabaseUrl = import.meta.env.VITE_SUPABASE_URL;
const supabaseAnonKey = import.meta.env.VITE_SUPABASE_ANON_KEY;

if (!supabaseUrl || !supabaseAnonKey) {
  console.error('Supabase URL or Anon Key is missing. Make sure to set VITE_SUPABASE_URL and VITE_SUPABASE_ANON_KEY in your .env file.');
  // Optionally throw an error or handle this case more gracefully
}

// Export the single Supabase client instance
export { supabase };

// Export a function to get the client instance if needed
export const getSupabaseClient = () => supabase;

console.log('Supabase client initialized'); 