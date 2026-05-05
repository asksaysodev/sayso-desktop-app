export const compressAudioFile = async (cafFilePath: string, outputPath: string) => {
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

export const uploadFileViaIPC = async (filePath: string, type: string, parentId: string, fileName: string, metadata = {}) => {
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

export const uploadBothFilesViaIPC = async ({ user, prospect, sessionId }: { user: { file: string, actualStartMs: number }, prospect: { file: string, actualStartMs: number }, sessionId: string }) => {
    const accessToken: string | null = await window.electron?.ipcRenderer?.invoke('auth:get-token') ?? null;

    if (!accessToken) {
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
        accessToken
    });
};

export const uploadFullRecording = async (recordingResult: { userFile: string, prospectFile: string, sessionId: string, prospectId: string, userActualStartMs: number, prospectActualStartMs: number }, metadata = {}) => {
    const { 
      userFile, 
      prospectFile, 
      sessionId, 
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