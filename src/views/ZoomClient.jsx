import React, { useState, useCallback, useEffect } from 'react';
import { useParams } from 'react-router-dom';
import { useAuth } from '../context/AuthContext';
import { useZoom } from '../hooks/useZoom';
import { ZoomMtg } from '@zoom/meetingsdk'
import { useNavigate } from 'react-router-dom';
import { useSalesCoachContext } from '../context/SalesCoachContext';
import '../styles/ZoomClient.css';

export const ZoomClient = () => {
  const [signature, setSignature] = useState(null);
  const [meetingDetails, setMeetingDetails] = useState(null);
  const [isLoading, setIsLoading] = useState(true);
  const [error, setError] = useState(null);
  const [permissionStatus, setPermissionStatus] = useState(null);
  const [cameraInitialized, setCameraInitialized] = useState(false);

  const navigate = useNavigate();
  const { meetingId, prospectId } = useParams();

  const { globalUser } = useAuth();
  const { requestMediaPermissions,  getMeetingDetails,  getSignature } = useZoom();
  const { iceBreaker , setIsZoomInitialized, setIsCallActive } = useSalesCoachContext();

  const isElectron = !!(window && window.electron && window.electron.ipcRenderer);

  // Compute leaveUrl for Electron to use full file URL
  let leaveUrl;
  if (isElectron && window.sayso?.indexHtmlPath) {
    leaveUrl = `https://google.com/?meetingId=${meetingId}&prospectId=${prospectId}&success=true`;
  } else {
    leaveUrl = `${import.meta.env.VITE_FRONTEND_BASE_URL}/post-call/${meetingId}/${prospectId}?success=true`;
  }

  // Initialize camera permissions early
  const initializeCamera = useCallback(async () => {
    try {
      console.log('🎥 Initializing camera permissions...');
      
      // Check if we're in Electron
      const isElectron = window.electron && window.electron.ipcRenderer;
      
      // Request camera permissions
      const stream = await navigator.mediaDevices.getUserMedia({
        audio: true,
        video: {
          width: { ideal: 1280 },
          height: { ideal: 720 },
          facingMode: 'user'
        }
      });
      
      stream.getTracks().forEach(track => track.stop());
      setCameraInitialized(true);
      return true;
    } catch (error) {
      console.error('❌ Camera initialization failed:', error);
      setError(`Camera access denied: ${error.message}`);
      return false;
    }
  }, []);

  const handleZoomInitialized = useCallback(() => {
    setTimeout(() => {
      setIsZoomInitialized(true);
    }, 5000);
  }, [setIsZoomInitialized]);

  const startMeeting = async (signature, sdkKey, meetingNumber, passWord, userName, userEmail) => {
    try {
      const rootElement = document.getElementById("zmmtg-root");
      if (rootElement) {
        rootElement.style.display = "block";
      }

      ZoomMtg.init({
        leaveUrl,
        success: () => {
          ZoomMtg.join({
            signature,
            sdkKey,
            meetingNumber,
            passWord,
            userName,
            userEmail,
            success: () => {
              setIsLoading(false);
              handleZoomInitialized();
            },
            error: (error) => {
              console.error("Zoom join error:", error);
              setError("Failed to join meeting. Please check your connection and try again.");
              setIsLoading(false);
            }
          });
        },
        error: (error) => {
          console.error("Zoom init error:", error);
          setError("Failed to initialize Zoom. Please refresh the page and try again.");
          setIsLoading(false);
        }
      });
    } catch (error) {
      console.error("Error starting meeting:", error);
      setError("An unexpected error occurred. Please try again.");
      setIsLoading(false);
    }
  };

  const getMeetingDetailsAndSignature = async () => {
    try {
      setIsLoading(true);
      setError(null);
      
      const meetingDetails = await getMeetingDetails(globalUser.id, meetingId);
      setMeetingDetails(meetingDetails);
      
      if (meetingDetails) {
        const signature = await getSignature(meetingDetails.id, 1);
        setSignature(signature);
        
        await startMeeting(
          signature.signature,
          signature.sdkKey,
          meetingDetails.id,
          meetingDetails.password,
          globalUser.name || "Sayso User",
          globalUser.email
        );
      }
    } catch (error) {
      console.error("❌ Error fetching meeting details:", error);
      setError("Failed to start meeting. Please try again.");
    } finally {
      setIsLoading(false);
    }
  };

  const handleManualPermission = async () => {
    try {
      console.log('🔐 Requesting media permissions...');
      const granted = await requestMediaPermissions();
      setPermissionStatus(granted ? 'granted' : 'denied');
      
      if (granted) {
        console.log('✅ Media permissions granted, initializing camera...');
        const cameraReady = await initializeCamera();
        if (cameraReady) {
          await getMeetingDetailsAndSignature();
        }
      } else {
        setError("Media permissions denied. Please allow camera and microphone access.");
      }
    } catch (error) {
      console.error("❌ Error requesting permissions:", error);
      setError("Failed to get media permissions. Please check your browser settings.");
    }
  };

  useEffect(() => {
    if (meetingId && globalUser) {
        console.log('🎯 Starting Zoom client with meetingId:', meetingId);
        console.log('👤 globalUser:', globalUser);
        handleManualPermission();
    }
  }, [meetingId, globalUser]);

  return (
    <div>
      {signature && meetingDetails && (
        <div className="flex items-center justify-center h-screen governmenthooka">
          <div id="zmmtg-root" style={{
            display: 'block',
            position: 'relative',
            zIndex: 9998,
            background: 'white',
            minWidth: '800px'
          }}></div>
          
        </div>
        
      )}
      
    </div>
  );
};
