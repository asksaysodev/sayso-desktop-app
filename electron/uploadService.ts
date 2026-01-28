import { UploadAudioChunkMetadata } from "./globals";
import axios from 'axios';
import NodeFormData from 'form-data';
import fs from 'fs';
import * as Sentry from '@sentry/electron/main';

class UploadService {
  private baseUrl: string;
  
  constructor() {
    this.baseUrl = process.env.VITE_BACKEND_BASE_URL || 'http://localhost:4000';
    console.log('[Upload Service] 🔍 DEBUG - Environment check:', {
      NODE_ENV: process.env.NODE_ENV,
      VITE_BACKEND_BASE_URL: process.env.VITE_BACKEND_BASE_URL,
      finalBaseUrl: this.baseUrl
    });
    console.log('[Upload Service] Initialized with baseUrl:', this.baseUrl);
    console.log('[Upload Service] Environment variables:', {
      NODE_ENV: process.env.NODE_ENV,
      VITE_BACKEND_BASE_URL: process.env.VITE_BACKEND_BASE_URL
    });
  }

  async uploadAudioChunk(filePath: string, speaker: string, metadata: UploadAudioChunkMetadata = {}) { 
    try {
      console.log(`[Upload Service] Starting upload for ${speaker} chunk: ${filePath}`);
      
      if (!fs.existsSync(filePath)) {
        throw new Error(`File not found: ${filePath}`);
      }

      const fileStats = fs.statSync(filePath);
      console.log(`[Upload Service] File stats for ${speaker} chunk:`, {
        size: fileStats.size,
        permissions: fileStats.mode,
        path: filePath
      });

      if (fileStats.size === 0) {
        throw new Error(`File is empty: ${filePath}`);
      }

      const formData = new NodeFormData();
      formData.append('audio', fs.createReadStream(filePath));
      formData.append('speaker', speaker);

      // Add metadata to query params
      const queryParams = new URLSearchParams();
      if (metadata.accountId) queryParams.append('accountId', metadata.accountId);
      if (metadata.prospectId) queryParams.append('prospectId', metadata.prospectId);
      if (metadata.meetingId) queryParams.append('meetingId', metadata.meetingId);
      if (metadata.sessionId) queryParams.append('sessionId', metadata.sessionId);

      const url = `${this.baseUrl}/audio/transcript?${queryParams.toString()}`;
      console.log(`[Upload Service] Uploading to: ${url}`);

      const response = await axios.post(url, formData, {
        headers: {
          ...formData.getHeaders(),
        },
        maxContentLength: Infinity,
        maxBodyLength: Infinity,
        timeout: 30000, // 30 second timeout
      });

      if (!response.data) {
        throw new Error('No response data received from server');
      }

      console.log(`[Upload Service] Upload successful for ${speaker} chunk:`, response.data);
      return response.data;
    } catch (error: any) {
      console.error('[Upload Service] Error uploading audio chunk:', {
        error: error.message,
        code: error.code,
        response: error.response?.data,
        status: error.response?.status,
        filePath,
        speaker,
        url: `${this.baseUrl}/audio/transcript`,
        headers: error.config?.headers
      });
      Sentry.captureException(error);

      // Create a more descriptive error message
      let errorMessage = `Failed to upload audio chunk: ${error.message}`;
      if (error.response?.data?.error) {
        errorMessage += ` - Server error: ${error.response.data.error}`;
      }
      if (error.code === 'ECONNREFUSED') {
        errorMessage = 'Could not connect to server. Please check if the server is running.';
      }
      if (error.code === 'ETIMEDOUT') {
        errorMessage = 'Upload timed out. Please try again.';
      }

      // Create a new error with the descriptive message
      const uploadError = new Error(errorMessage) as UploadError;
      uploadError.originalError = error;
      uploadError.filePath = filePath;
      uploadError.speaker = speaker;
      throw uploadError;
    }
  }
}

export default new UploadService(); 