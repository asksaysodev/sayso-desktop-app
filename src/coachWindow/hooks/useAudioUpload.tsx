import { useState } from 'react';
import * as Sentry from "@sentry/electron/renderer";

export const useAudioUpload = () => {
  const [isCompressing, setIsCompressing] = useState(false);
  const [isUploading, setIsUploading] = useState(false);

  const compressAudioFile = async (cafFilePath: string, outputPath: string) => {
    // Call Electron to compress via FFmpeg
    if (window.electron && window.electron.recording) {
      return await window.electron.recording.compressAudio({
        inputPath: cafFilePath,
        outputPath: outputPath,
        format: 'mp3',
        bitrate: '128k', // Adjust as needed
        sampleRate: 48000
      });
    }
    throw new Error('Electron compression API not available');
  };

  const uploadFileViaIPC = async (filePath: string, type: string, parentId: string, fileName: string, metadata = {}) => {
    const accessToken: string | null = await window.electron?.ipcRenderer?.invoke('auth:get-token') ?? null;

    if (!accessToken) {
      throw new Error('No authentication token available. Please log in.');
    }

    if (!window.electron?.uploadFile) {
      throw new Error('Electron upload API not available');
    }

    return await window.electron.uploadFile({
      filePath,
      type,
      parentId,
      accessToken,
      fileName,
      data: metadata
    });
  };

  const uploadBothFilesViaIPC = async ({ user, prospect, sessionId }: { user: { file: string, actualStartMs: number }, prospect: { file: string, actualStartMs: number }, sessionId: string }) => {
    const accessToken: string | null = await window.electron?.ipcRenderer?.invoke('auth:get-token') ?? null;

    if (!accessToken) {
      throw new Error('No authentication token available. Please log in.');
    }

    if (!window.electron?.uploadBothFiles) {
      throw new Error('Electron upload API not available');
    }

    if (!user?.file || user?.actualStartMs === undefined) {
      throw new Error('User file and actualStartMs are required');
    }

    if (!prospect?.file || prospect?.actualStartMs === undefined) {
      throw new Error('Prospect file and actualStartMs are required');
    }

    if (!sessionId) {
      throw new Error('sessionId is required');
    }

    return await window.electron.uploadBothFiles({
      user: {
        file: user.file,
        actualStartMs: user.actualStartMs
      },
      prospect: {
        file: prospect.file,
        actualStartMs: prospect.actualStartMs
      },
      sessionId,
      accessToken
    });
  };

  const uploadFullRecording = async (recordingResult: { userFile: string, prospectFile: string, sessionId: string, prospectId: string, userActualStartMs: number, prospectActualStartMs: number }, metadata = {}) => {
    try {
      setIsCompressing(true);
      
      const { userFile, prospectFile, sessionId, prospectId, userActualStartMs, prospectActualStartMs } = recordingResult;
      
      // Compress both files in parallel
      const userCompressed = userFile.replace('.caf', '.mp3');
      const prospectCompressed = prospectFile.replace('.caf', '.mp3');
      
      await Promise.all([
        compressAudioFile(userFile, userCompressed),
        compressAudioFile(prospectFile, prospectCompressed)
      ]);
      
      setIsCompressing(false);
      setIsUploading(true);
      
      // Upload both files together using the new function
      const result = await uploadBothFilesViaIPC({
        user: {
          file: userCompressed,
          actualStartMs: userActualStartMs
        },
        prospect: {
          file: prospectCompressed,
          actualStartMs: prospectActualStartMs
        },
        sessionId
      });
      
      setIsUploading(false);
      
      return {
        ...result,
        sessionId,
        metadata: {
          userActualStartMs,
          prospectActualStartMs
        }
      };
      
    } catch (error) {
      setIsCompressing(false);
      setIsUploading(false);
      Sentry.captureException(error);
      throw error;
    }
  };

  return {
    compressAudioFile, // Export so it can be used standalone after stopDualChannelRecording
    uploadFullRecording,
    uploadBothFilesViaIPC, // Export the new function
    isCompressing,
    isUploading
  };
};