/**
 * Type definitions for Electron main process globals
 * These extend NodeJS.Global with app-specific properties
 */

import type { BrowserWindow } from 'electron';
import type { ChildProcess } from 'child_process';

// Account type for authenticated user (simplified - main process only needs basic info)
interface AuthUser {
  id: string;
  email: string;
  subscription_plan_id: string | null;
  [key: string]: unknown;
}

declare global {
  // Custom error type for file uploads
  interface UploadError extends Error {
    originalError?: unknown;
    filePath?: string;
  }
  var coachWindow: BrowserWindow | null;
  var mainWindow: BrowserWindow | null;
  var authUser: AuthUser | false | null;

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
