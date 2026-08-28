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
  onboarding_status?: 'complete' | 'dismissed' | null;
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
  var playbookWindow: BrowserWindow | null;
  var appSettingsWindow: BrowserWindow | null;
  var appSettingsWindowSource: 'coach' | 'independent' | null;
  var playbookWindowSource: 'coach' | 'independent' | null;
  var mainWindow: BrowserWindow | null;
  var authUser: AuthUser | false | null;
  var authAccessToken: string | null;
  var authRefreshToken: string | null;
  var networkState: 'online' | 'reconnecting';

  // Audio streaming process
  var userStreamingProcess: ChildProcess | null;
}

export {};
