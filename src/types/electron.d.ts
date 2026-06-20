/**
 * Type definitions for Electron IPC bridge
 * These types match the API exposed in electron/preload.js
 */

import { UpdateState } from './update';

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

export interface CueSmartCaptureItem {
  topic: string;
  content: string;
}

export interface CueAPI {
  start: (params: CueParams) => Promise<CueResult>;
  stop: () => Promise<boolean>;
  onStatus: (callback: (data: CueStatusData) => void) => () => void;
  onInsight: (callback: (data: CueInsightData) => void) => () => void;
  onError: (callback: (data: CueErrorData) => void) => () => void;
  onAutoStop: (callback: (data: unknown) => void) => () => void;
  onLowUserAudio: (callback: (data: CueLowUserAudioData) => void) => () => void;
  onSmartCapture: (callback: (data: CueSmartCaptureItem[]) => void) => () => void;
}


export interface PermissionsAPI {
  check: () => Promise<PermissionsStatus>;
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
}

export interface UpdateAPI {
  getState: () => Promise<UpdateState>;
  onStateChanged: (callback: (state: UpdateState) => void) => () => void;
  startDownload: () => void;
  dismiss: () => void;
  checkForUpdates: () => void;
}

export interface AppAPI {
  getVersion: () => Promise<string>;
}

export interface AppSettingsAPI {
  openUpdateTab: () => void;
}

export interface ElectronBridge {
  ipcRenderer: ElectronIpcRenderer;
  openExternal: (url: string) => void;
  nativeAudio: NativeAudioAPI;
  cue: CueAPI;
  permissions: PermissionsAPI;
  autoUpdater: AutoUpdaterAPI;
  update: UpdateAPI;
  app: AppAPI;
  appSettings: AppSettingsAPI;
}

export interface PlaybookWindowAPI {
  getWindowPosition: () => Promise<[number, number]>;
  setWindowPosition: (x: number, y: number) => void;
}

export interface ElectronAPIBridge {
  resizeWindow: (width: number, height: number) => void;
  closeCoachWindow: () => void;
  getCoachWorkAreaBottom: () => Promise<number | null>;
  getWindowPosition: () => Promise<[number, number]>;
  setWindowPosition: (x: number, y: number) => void;
  playbook: PlaybookWindowAPI;
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
