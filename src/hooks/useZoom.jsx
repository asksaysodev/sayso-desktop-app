import { useState, useCallback } from 'react';
import { useAuth } from '../context/AuthContext';
import axios from 'axios';

export const useZoom = () => {

    // const { globalUser, authToken } = useAuth();
    const [mediaPermissions, setMediaPermissions] = useState({
        audio: false,
        video: false
    });

    const getZoomMeetings = async (userId) => {
        const response = await axios.get(`${import.meta.env.VITE_BACKEND_BASE_URL}/zoom/meetings/${userId}`);
        return response.data;
    }

    // Enhanced media permissions request
    const requestMediaPermissions = useCallback(async () => {
        try {
            
            // Check if we're in Electron
            const isElectron = window.electron && window.electron.ipcRenderer;
            
            // Check if mediaDevices is available
            if (!navigator.mediaDevices) {
                console.error('❌ navigator.mediaDevices is not available');
                throw new Error('Media devices not supported');
            }
            
            // Request permissions with better video constraints
            const stream = await navigator.mediaDevices.getUserMedia({
                audio: {
                    echoCancellation: true,
                    noiseSuppression: true,
                    autoGainControl: true
                },
                video: {
                    width: { ideal: 1280, min: 640 },
                    height: { ideal: 720, min: 480 },
                    frameRate: { ideal: 30, min: 15 },
                    facingMode: 'user'
                }
            });
        
            
            // Stop the test stream
            stream.getTracks().forEach(track => track.stop());
            
            setMediaPermissions({ audio: true, video: true });
            return true;
        } catch (error) {
            console.error('❌ Error requesting media permissions:', error);
            
            // Provide more specific error messages
            let errorMessage = 'Unknown error';
            if (error.name === 'NotAllowedError') {
                errorMessage = 'Camera/microphone access denied by user';
            } else if (error.name === 'NotFoundError') {
                errorMessage = 'No camera or microphone found';
            } else if (error.name === 'NotReadableError') {
                errorMessage = 'Camera or microphone is already in use';
            } else if (error.name === 'OverconstrainedError') {
                errorMessage = 'Camera does not meet the required constraints';
            } else if (error.name === 'TypeError') {
                errorMessage = 'Media devices not supported';
            }
            
            console.error(`❌ Permission error: ${errorMessage}`);
            setMediaPermissions({ audio: false, video: false });
            return false;
        }
    }, []);

    const getMeetingDetails = useCallback(async (userId, meetingId) => {

        if(!userId || !meetingId) {
            throw new Error('User ID and meeting ID are required');
        }

        try {
            const response = await axios.get(`${import.meta.env.VITE_BACKEND_BASE_URL}/zoom/meeting-details/${userId}/${meetingId}`);
            return response.data;

        } catch (error) {
            console.error('❌ Error getting meeting details:', error);
        }

    }, []);

    const disconnectZoom = useCallback(async (userId) => {
        const response = await axios.post(`${import.meta.env.VITE_BACKEND_BASE_URL}/zoom/disconnect/${userId}`);
        return response.data;
    }, []);

    const getSignature = useCallback(async (meetingNumber, role) => {
        if(!meetingNumber || !role) {
            throw new Error('Meeting number and role are required');
        }

        try {
            const response = await axios.post(`${import.meta.env.VITE_BACKEND_BASE_URL}/zoom/signature`, { meetingNumber, role });
            return response.data;
        } catch (error) {
            console.error('❌ Error getting signature:', error);
        }
    }, []);

    return {
        requestMediaPermissions,
        mediaPermissions,
        getMeetingDetails,
        getSignature,   
        getZoomMeetings,
        disconnectZoom
    };
};
