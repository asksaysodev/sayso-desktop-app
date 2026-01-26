/**
 * Audio Recording Service
 * Handles Electron IPC communication for dual-channel audio recording
 */

import { AudioRecordingParams } from "@/types/electron";
import * as Sentry from "@sentry/electron/renderer";

/**
 * Stops the dual-channel recording via Electron API
 * @returns {Promise<Object>} Recording result with file paths
 * @throws {Error} If Electron recording API is not available
 */
export const stopDualChannelRecording = async (): Promise<boolean> => {
    console.log('🎤 [audioRecordingService] Stopping dual channel recording...');
    
    if (window.electron && window.electron.recording) {
        try {
            const result = await window.electron.recording.stopDualChannel();
            console.log('🎤 [audioRecordingService] ✅ Recording stopped successfully:', result);
            return result;
        } catch (error) {
            console.error('🎤 [audioRecordingService] ❌ Error stopping recording:', error);
            Sentry.captureException(error);
            throw error;
        }
    } else {
        console.warn('🎤 [audioRecordingService] Electron recording API is not available. Cannot stop recording.');
        throw new Error('Electron recording API not available');
    }
};

/**
 * Starts dual-channel recording via Electron API
 * @param {AudioRecordingParams} recordingParams - Recording parameters
 * @returns {Promise<boolean>} Recording start result
 * @throws {Error} If Electron recording API is not available
 */
export const startDualChannelRecording = async (recordingParams: AudioRecordingParams): Promise<boolean> => {
    console.log('🎤 [audioRecordingService] Starting dual channel recording...', recordingParams);
    
    if (window.electron && window.electron.recording) {
        try {
            const result = await window.electron.recording.startDualChannel(recordingParams);
            console.log('🎤 [audioRecordingService] ✅ Recording started successfully:', result);
            return result;
        } catch (error) {
            console.error('🎤 [audioRecordingService] ❌ Error starting recording:', error);
            Sentry.captureException(error);
            throw error;
        }
    } else {
        console.error('[audioRecordingService] Electron recording API is not available!');
        console.error('[audioRecordingService] window.electron:', window.electron);
        Sentry.captureMessage('Electron recording API not available', 'error');
        throw new Error('Electron recording API not available');
    }
};