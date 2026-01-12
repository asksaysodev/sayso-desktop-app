import axios from 'axios';
import { supabase } from './supabase';
import * as Sentry from "@sentry/electron/renderer";

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

    // Add retry tracking
    config._retryCount = config._retryCount || 0;

    Sentry.addBreadcrumb({
      category: 'api.request',
      message: `${config.method?.toUpperCase()} ${config.url}`,
      level: 'info',
      data: {
        method: config.method,
        url: config.url,
        baseURL: config.baseURL,
        retryCount: config._retryCount
      }
    });

    return config;
  },
  (error) => {
    return Promise.reject(error);
  }
);

// Response interceptor to handle 401 errors and retry logic
apiClient.interceptors.response.use(
  (response) => {
    console.log('🌐 [API Response]', {
      url: response.config.url,
      status: response.status,
      data: response.data
    });

    Sentry.addBreadcrumb({
      category: 'api.response',
      message: `${response.config.method?.toUpperCase()} ${response.config.url} - ${response.status}`,
      level: 'info',
      data: {
        method: response.config.method,
        url: response.config.url,
        status: response.status,
        statusText: response.statusText
      }
    });

    return response;
  },
  async (error) => {
    const config = error.config;

    console.error('🌐 [API Error]', {
      url: config?.url,
      status: error.response?.status,
      data: error.response?.data,
      code: error.code,
      retryCount: config?._retryCount
    });

    Sentry.addBreadcrumb({
      category: 'api.error',
      message: `${config?.method?.toUpperCase()} ${config?.url} - ${error.response?.status || error.code}`,
      level: 'error',
      data: {
        method: config?.method,
        url: config?.url,
        status: error.response?.status,
        statusText: error.response?.statusText,
        code: error.code,
        retryCount: config?._retryCount,
        errorData: error.response?.data
      }
    });

    // Retry logic for network errors and empty responses
    if (
      (error.code === 'ERR_NETWORK' || error.code === 'ERR_EMPTY_RESPONSE') &&
      config &&
      config._retryCount < 3
    ) {
      config._retryCount = (config._retryCount || 0) + 1;

      console.log(`🔄 [API Retry] Attempt ${config._retryCount}/3 for ${config.url}`);

      Sentry.addBreadcrumb({
        category: 'api.retry',
        message: `Retrying ${config.method?.toUpperCase()} ${config.url} (Attempt ${config._retryCount}/3)`,
        level: 'warning',
        data: {
          method: config.method,
          url: config.url,
          retryCount: config._retryCount,
          reason: error.code
        }
      });

      // Wait before retrying (exponential backoff)
      await new Promise(resolve => setTimeout(resolve, 1000 * config._retryCount));

      return apiClient(config);
    }

    return Promise.reject(error);
  }
);

export default apiClient; 