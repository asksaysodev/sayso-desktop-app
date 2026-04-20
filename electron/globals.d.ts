/**
 * Type definitions for Electron main process globals
 * These extend NodeJS.Global with app-specific properties
 */

import type { BrowserWindow } from 'electron';
import type { ChildProcess } from 'child_process';

// (simplified - main process only needs basic info)
export interface AuthUser {
  id: string;
  email: string;
  subscription_plan_id: string | null;
  [key: string]: unknown;
}

export interface AudioQueueItem {
  filePath: string;
  speaker: string;
  retries: number;
  status: string;
  timestamp: number;
  onChunk: (filePath: string, speaker: string) => Promise<void>;
}

// Audio capture options for native audio module
export interface AudioCaptureOptions {
  streamingCallback?: (audioData: Buffer, sampleRate: number) => void;
  streamingOnly?: boolean;
  [key: string]: unknown;
}

export interface CueInsight {
  insight: string;
  timestamp: number;
  message: string;
  priority: number;
  appointmentBooked: boolean;
  id: string;
}

export interface CueParams {
  sessionId: string;
  token: string;
}

export interface UploadFileOptions {
  filePath: string;
  type: string;
  parentId: string;
  accessToken: string;
  fileName: string;
  data?: Record<string, unknown>;
  contentType?: string;
}

export interface UploadBothFilesOptions {
  user: {
    file: string;
    actualStartMs: number;
  };
  prospect: {
    file: string;
    actualStartMs: number;
  };
  sessionId: string;
  accessToken: string;
}

export interface UploadAudioChunkMetadata {
  accountId?: string;
  prospectId?: string;
  meetingId?: string;
  sessionId?: string;
}

declare global {
  // Custom error type for file uploads
  interface UploadError extends Error {
    originalError?: unknown;
    filePath?: string;
    speaker?: string;
  }
  var coachWindow: BrowserWindow | null;
  var appSettingsWindow: BrowserWindow | null;
  var mainWindow: BrowserWindow | null;
  var authUser: AuthUser | false | null;
  var authAccessToken: string | null;
  var authRefreshToken: string | null;

  // Audio recording processes
  var userStreamingProcess: ChildProcess | null;
  var userFullRecordingProcess: ChildProcess | null;

  // Media recorders (handles from renderer, typed loosely for main process)
  var userMediaRecorder: { stop: () => void } | null;
  var mediaRecorder: { stop: () => void } | null;

  // Audio streams (handles from renderer, typed loosely for main process)
  var userAudioStream: { getTracks: () => { stop: () => void }[] } | null;
  var prospectAudioStream: { getTracks: () => { stop: () => void }[] } | null;

  // Screen capture instance
  var screenCapture: { stopSystemAudioCapture: () => Promise<void> } | null;

  // Recording file paths
  var userRecordingFile: string | null;
  var prospectRecordingFile: string | null;
  var userActualStartMs: number | null;
}

export {};
