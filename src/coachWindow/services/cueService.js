import { supabase } from "../../config/supabase";

export const cue_startStreaming = async (sessionId) => {
    try {
        if(!sessionId) {
            throw new Error('Session ID is required');
        }
        
        if (!window.electron?.cue) {
            throw new Error('Electron cue API not available');
        }
        
        const { data: { session }} = await supabase.auth.getSession();
        
        if(!session || !session.access_token) {
            throw new Error('No session found');
        }

        const result = await window.electron.cue.start({ 
            sessionId, 
            token: session.access_token 
        });

        if(!result || !result.success) {
            throw new Error('Failed to start cue streaming');
        }

        return result;

    } catch (error) {
        console.error('Error starting cue streaming:', error);
        throw error;
    }
}

export const cue_stopStreaming = async () => {
    try {
        if (!window.electron?.cue) {
            throw new Error('Electron cue API not available');
        }

        await window.electron.cue.stop();

    } catch (error) {
        console.error('Error stopping cue streaming:', error);
        throw error;
    }
}