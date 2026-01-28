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
const axios_1 = __importDefault(require("axios"));
const form_data_1 = __importDefault(require("form-data"));
const fs_1 = __importDefault(require("fs"));
const Sentry = __importStar(require("@sentry/electron/main"));
class UploadService {
    baseUrl;
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
    async uploadAudioChunk(filePath, speaker, metadata = {}) {
        try {
            console.log(`[Upload Service] Starting upload for ${speaker} chunk: ${filePath}`);
            if (!fs_1.default.existsSync(filePath)) {
                throw new Error(`File not found: ${filePath}`);
            }
            const fileStats = fs_1.default.statSync(filePath);
            console.log(`[Upload Service] File stats for ${speaker} chunk:`, {
                size: fileStats.size,
                permissions: fileStats.mode,
                path: filePath
            });
            if (fileStats.size === 0) {
                throw new Error(`File is empty: ${filePath}`);
            }
            const formData = new form_data_1.default();
            formData.append('audio', fs_1.default.createReadStream(filePath));
            formData.append('speaker', speaker);
            // Add metadata to query params
            const queryParams = new URLSearchParams();
            if (metadata.accountId)
                queryParams.append('accountId', metadata.accountId);
            if (metadata.prospectId)
                queryParams.append('prospectId', metadata.prospectId);
            if (metadata.meetingId)
                queryParams.append('meetingId', metadata.meetingId);
            if (metadata.sessionId)
                queryParams.append('sessionId', metadata.sessionId);
            const url = `${this.baseUrl}/audio/transcript?${queryParams.toString()}`;
            console.log(`[Upload Service] Uploading to: ${url}`);
            const response = await axios_1.default.post(url, formData, {
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
        }
        catch (error) {
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
            const uploadError = new Error(errorMessage);
            uploadError.originalError = error;
            uploadError.filePath = filePath;
            uploadError.speaker = speaker;
            throw uploadError;
        }
    }
}
exports.default = new UploadService();
//# sourceMappingURL=uploadService.js.map