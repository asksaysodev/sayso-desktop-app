/**
 * Type definitions for Electron IPC bridge
 * These types match the API exposed in electron/preload.js
 */

export interface ElectronIpcRenderer {
  invoke: <T = unknown>(channel: string, ...args: unknown[]) => Promise<T>;
  on: (channel: string, callback: (data: unknown) => void) => () => void;
  send: (channel: string, ...args: unknown[]) => void;
  removeAllListeners: (channel: string) => void;
  off: (channel: string, callback: (...args: unknown[]) => void) => void;
}

export interface NativeAudioDevice {
  id: string;
  name: string;
  isInput: boolean;
}

export interface NativeAudioAPI {
  initialize: () => Promise<boolean>;
  listDevices: () => Promise<NativeAudioDevice[]>;
  createDevice: (name: string, subDevices: string[]) => Promise<{ deviceId: string }>;
  deleteDevice: (deviceId: string) => Promise<boolean>;
  requestPermission: () => Promise<boolean>;
  startCapture: (options: AudioCaptureOptions) => Promise<boolean>;
  stopCapture: () => Promise<boolean>;
  isCapturing: () => Promise<boolean>;
}

export interface AudioCaptureOptions {
  deviceId?: string;
  sampleRate?: number;
  channels?: number;
}

export interface RecordingAPI {
  startDualChannel: (params: AudioRecordingParams) => Promise<boolean>;
  stopDualChannel: () => Promise<boolean>;
  compressAudio: (options: CompressAudioOptions) => Promise<string>;
}

export interface AudioRecordingParams {
  sessionId: string;
  prospectId: string;
  metadata: {
    sessionId: string;
    prospectId: string;
    timestamp: number;
  };
}

export interface CompressAudioOptions {
  format?: string;
  inputPath: string;
  outputPath: string;
  bitrate?: string;
  sampleRate?: number;
}

export interface CueParams {
  sessionId: string;
  token: string;
}

export interface CueStatusData {
  status: 'connecting' | 'connected' | 'disconnected' | 'error';
  message?: string;
  stream?: string;
}

export interface CueInsightData {
  insight: string;
  timestamp: number;
  message: string;
  priority: number;
  appointmentBooked: boolean;
  id: string;
}

export interface CueErrorData {
  error: string;
  stream?: string;
  code?: string;
}

export interface CueResult {
  success: boolean;
  message?: string;
  error?: string;
}

export interface CueLowUserAudioData {
  sessionId: string;
}

export interface CueAPI {
  start: (params: CueParams) => Promise<CueResult>;
  stop: () => Promise<boolean>;
  onStatus: (callback: (data: CueStatusData) => void) => () => void;
  onInsight: (callback: (data: CueInsightData) => void) => () => void;
  onError: (callback: (data: CueErrorData) => void) => () => void;
  onAutoStop: (callback: (data: unknown) => void) => () => void;
  onLowUserAudio: (callback: (data: CueLowUserAudioData) => void) => () => void;
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
  user: { file: string, actualStartMs: number };
  prospect: { file: string, actualStartMs: number };
  sessionId: string;
  accessToken: string;
}

export interface PermissionsAPI {
  check: () => Promise<PermissionsStatus>;
  requestAll: () => Promise<PermissionsStatus>;
}

export interface PermissionsStatus {
  mic: boolean;
  screen: boolean;
}

export interface AutoUpdaterDownloadProgress {
  percent: number;
  bytesPerSecond: number;
  transferred: number;
  total: number;
}

export interface AutoUpdaterAPI {
  onUpdateCheckComplete: (callback: () => void) => () => void;
  onUpdateAvailable: (callback: (data: { version: string }) => void) => () => void;
  onDownloadProgress: (callback: (data: AutoUpdaterDownloadProgress) => void) => () => void;
  onUpdateDownloaded: (callback: (data: { version: string }) => void) => () => void;
  installUpdate: () => void;
}

export interface ElectronBridge {
  ipcRenderer: ElectronIpcRenderer;
  openExternal: (url: string) => void;
  nativeAudio: NativeAudioAPI;
  recording: RecordingAPI;
  cue: CueAPI;
  uploadFile: (options: UploadFileOptions) => Promise<{ success: boolean; error?: string }>;
  uploadBothFiles: (options: UploadBothFilesOptions) => Promise<{ success: boolean; error?: string }>;
  permissions: PermissionsAPI;
  autoUpdater: AutoUpdaterAPI;
}

export interface ElectronAPIBridge {
  resizeWindow: (width: number, height: number) => void;
  closeCoachWindow: () => void;
  getWindowPosition: () => Promise<[number, number]>;
  setWindowPosition: (x: number, y: number) => void;
}

export interface SaysoBridge {
  indexHtmlPath: string;
}

// Extend the global Window interface
declare global {
  interface Window {
    electron?: ElectronBridge;
    electronAPI?: ElectronAPIBridge;
    sayso?: SaysoBridge;
    debugStorage?: () => void;
  }
}

export {};
