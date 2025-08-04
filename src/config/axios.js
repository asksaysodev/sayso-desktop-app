import axios from 'axios';
import { supabase } from './supabase';

// Create axios instance with base configuration
const apiClient = axios.create({
  baseURL: import.meta.env.VITE_BACKEND_BASE_URL,
  timeout: 10000,
});

// Request interceptor to add auth token
apiClient.interceptors.request.use(
  async (config) => {
    // Get the current session
    const { data: { session } } = await supabase.auth.getSession();
    
    if (session?.access_token) {
      config.headers.Authorization = `Bearer ${session.access_token}`;
    }
    
    return config;
  },
  (error) => {
    return Promise.reject(error);
  }
);

// Response interceptor to handle 401 errors
apiClient.interceptors.response.use(
  (response) => {
    return response;
  },
  async (error) => {
    const originalRequest = error.config;

    // If we get a 401 and haven't already tried to refresh
    if (error.response?.status === 401 && !originalRequest._retry) {
      originalRequest._retry = true;

      try {
        // Try to refresh the session
        const { data: { session }, error: refreshError } = await supabase.auth.refreshSession();
        
        if (refreshError || !session) {
          // If refresh fails, sign out the user
          console.log('🔐 Session expired, signing out user...');
          await supabase.auth.signOut();
          
          // Dispatch a custom event to notify the app
          window.dispatchEvent(new CustomEvent('auth:session-expired'));
          
          return Promise.reject(error);
        }

        // Retry the original request with the new token
        originalRequest.headers.Authorization = `Bearer ${session.access_token}`;
        return apiClient(originalRequest);
        
      } catch (refreshError) {
        console.error('🔐 Error refreshing session:', refreshError);
        await supabase.auth.signOut();
        window.dispatchEvent(new CustomEvent('auth:session-expired'));
        return Promise.reject(error);
      }
    }

    return Promise.reject(error);
  }
);

export default apiClient; 