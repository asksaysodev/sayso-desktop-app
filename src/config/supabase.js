import { createClient } from '@supabase/supabase-js'
import { cryptoStorage, fallbackStorage } from '../utils/tokenEncryption'
import * as Sentry from "@sentry/electron/renderer"

const supabaseUrl = import.meta.env.VITE_SUPABASE_URL
const supabaseAnonKey = import.meta.env.VITE_SUPABASE_ANON_KEY

if (!supabaseUrl || !supabaseAnonKey) {
  console.error('Supabase URL or Anon Key is missing. Make sure to set VITE_SUPABASE_URL and VITE_SUPABASE_ANON_KEY in your .env file.');
}

// Create a robust storage wrapper that handles errors gracefully
const robustStorage = {
  async setItem(key, value) {
    try {
      await cryptoStorage.setItem(key, value);
    } catch (error) {
      console.warn('Crypto storage failed, using fallback:', error.message);
      Sentry.captureException(error);
      fallbackStorage.setItem(key, value);
    }
  },
  
  async getItem(key) {
    try {
      return await cryptoStorage.getItem(key);
    } catch (error) {
      console.warn('Crypto storage failed, using fallback:', error.message);
      Sentry.captureException(error);
      return fallbackStorage.getItem(key);
    }
  },
  
  removeItem(key) {
    try {
      cryptoStorage.removeItem(key);
    } catch (error) {
      console.warn('Crypto storage failed, using fallback:', error.message);
      Sentry.captureException(error);
      fallbackStorage.removeItem(key);
    }
  }
};

// Create a single instance of the Supabase client
export const supabase = createClient(supabaseUrl, supabaseAnonKey, {
  auth: {
    persistSession: true,
    storageKey: 'sayso-auth',
    // Use robust storage that handles errors gracefully
    storage: robustStorage
  }
}); 