import { supabase } from '../../config/supabase';

export const compressAudioFile = async (cafFilePath, outputPath) => {
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

export const uploadFileViaIPC = async (filePath, type, parentId, fileName, metadata = {}) => {
    const { data: { session } } = await supabase.auth.getSession();
    
    if (!session?.access_token) {
        throw new Error('No authentication token available. Please log in.');
    }
    
    if (!window.electron?.uploadFile) {
        throw new Error('Electron upload API not available');
    }
    
    return await window.electron.uploadFile({
        filePath,
        type,
        parentId,
        accessToken: session.access_token,
        fileName,
        data: metadata
    });
};

export const uploadBothFilesViaIPC = async ({ user, prospect, sessionId }) => {
    const { data: { session } } = await supabase.auth.getSession();
    
    if (!session?.access_token) {
        throw new Error('No authentication token available. Please log in.');
    }
    
    if (!window.electron?.uploadBothFiles) {
        throw new Error('Electron uploadBothFiles API not available');
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
        accessToken: session.access_token
    });
};

export const uploadFullRecording = async (recordingResult, metadata = {}) => {
    const { 
      userFile, 
      prospectFile, 
      sessionId, 
      prospectId, 
      userActualStartMs, 
      prospectActualStartMs 
    } = recordingResult;

    // this is returning typeerror because userFile is undefined
    // TO DO: we need to check why is undefined
    const userCompressed = userFile.replace('.caf', '.mp3');
    const prospectCompressed = prospectFile.replace('.caf', '.mp3');
    
    await Promise.all([
      compressAudioFile(userFile, userCompressed),
      compressAudioFile(prospectFile, prospectCompressed)
    ]);
    
    const result = await uploadBothFilesViaIPC({
      user: { file: userCompressed, actualStartMs: userActualStartMs },
      prospect: { file: prospectCompressed, actualStartMs: prospectActualStartMs },
      sessionId
    });
    
    return {
      ...result,
      sessionId,
      metadata: { userActualStartMs, prospectActualStartMs }
    };
  };