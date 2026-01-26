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
  startDualChannel: (params: DualChannelParams) => Promise<boolean>;
  stopDualChannel: () => Promise<boolean>;
  compressAudio: (options: CompressAudioOptions) => Promise<string>;
}

export interface DualChannelParams {
  micDeviceId?: string;
  systemAudioDeviceId?: string;
  outputPath?: string;
}

export interface CompressAudioOptions {
  inputPath: string;
  outputPath: string;
  bitrate?: string;
}

export interface CueParams {
  prospectId: string;
  accountId: string;
  authToken: string;
  callId?: string;
}

export interface CueStatusData {
  status: 'connecting' | 'connected' | 'disconnected' | 'error';
  message?: string;
}

export interface CueInsightData {
  insight: string;
  timestamp: number;
}

export interface CueErrorData {
  error: string;
  code?: string;
}

export interface CueAPI {
  start: (params: CueParams) => Promise<boolean>;
  stop: () => Promise<boolean>;
  onStatus: (callback: (data: CueStatusData) => void) => () => void;
  onInsight: (callback: (data: CueInsightData) => void) => () => void;
  onError: (callback: (data: CueErrorData) => void) => () => void;
  onAutoStop: (callback: (data: unknown) => void) => () => void;
}

export interface UploadFileOptions {
  filePath: string;
  uploadUrl: string;
  contentType?: string;
}

export interface UploadBothFilesOptions {
  micFilePath: string;
  systemFilePath: string;
  micUploadUrl: string;
  systemUploadUrl: string;
}

export interface PermissionsAPI {
  check: () => Promise<PermissionsStatus>;
  requestAll: () => Promise<PermissionsStatus>;
}

export interface PermissionsStatus {
  microphone: boolean;
  screenCapture: boolean;
  accessibility: boolean;
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
}

export interface ElectronAPIBridge {
  resizeWindow: (width: number, height: number) => void;
  closeCoachWindow: () => void;
  getWindowPosition: () => Promise<{ x: number; y: number }>;
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
