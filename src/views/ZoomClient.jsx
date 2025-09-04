import React, { useState, useCallback, useEffect } from 'react';
import { useParams } from 'react-router-dom';
import { useAuth } from '../context/AuthContext';
import { useZoom } from '../hooks/useZoom';
import { ZoomMtg } from '@zoom/meetingsdk'
import { useSalesCoachContext } from '../context/SalesCoachContext';
import '../styles/ZoomClient.css';


export const ZoomClient = () => {
  const [signature, setSignature] = useState(null);
  const [meetingDetails, setMeetingDetails] = useState(null);
  const [isLoading, setIsLoading] = useState(true);
  const [error, setError] = useState(null);
  const [permissionStatus, setPermissionStatus] = useState(null);
  const [cameraInitialized, setCameraInitialized] = useState(false);

  const { meetingId, prospectId, sessionId } = useParams();
  const { globalUser } = useAuth();
  const { requestMediaPermissions, getMeetingDetails, getSignature, getZak } = useZoom();
  const { iceBreaker, setIsZoomInitialized, setIsCallActive } = useSalesCoachContext();

  const isElectron = !!(window && window.electron && window.electron.ipcRenderer);

  // Compute leaveUrl for Electron to use full file URL
  let leaveUrl;
  if (isElectron && window.sayso?.indexHtmlPath) {
    leaveUrl = `${import.meta.env.VITE_BACKEND_BASE_URL}/post-call/?meetingId=${meetingId}&prospectId=${prospectId}&sessionId=${sessionId}&success=true`;
  } else {
    leaveUrl = `${import.meta.env.VITE_FRONTEND_BASE_URL}/post-call/${meetingId}/${prospectId}/${sessionId}?success=true`;
  }

  console.log('🔍 Leave URL:', leaveUrl);

  // Initialize camera permissions early
  const initializeCamera = useCallback(async () => {
    try {
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

  const startMeeting = useCallback(async (signature, sdkKey, meetingNumber, passWord, userName, userEmail, zak) => {
    try {
      document.getElementById('zmmtg-root').style.display = 'block';

      ZoomMtg.preLoadWasm();
      ZoomMtg.prepareWebSDK();

      ZoomMtg.init({
        leaveUrl,                  
        disablePreview: true,
        isSupportAV: false,
        customizeUI: true,
        leaveOnPageUnload: true,
        debug: true,
        defaultView: 'speaker',

        
        onRetryCallback: () => {
          console.log('[Zoom] Retry callback triggered');
        },
        
        success: () => {
          console.log('[Zoom] Init success, joining meeting...');
          ZoomMtg.join({
            signature,
            sdkKey,
            meetingNumber,
            passWord,
            userName,
            userEmail,
            zak,
            success: (res) => console.log('Meeting joined', res),
            error: (err) => console.error('Join Error', err),
        });

          
        },
        
        error: (error) => {
          console.error('[Zoom] Init error:', error);
        }
      });

      // ... rest of your existing code
    } catch (error) {
      console.error('[Zoom] Start meeting error:', error);
    }
  }, []);

  const getMeetingDetailsAndSignature = async () => {
    try {
      setIsLoading(true);
      setError(null);

      const meetingDetails = await getMeetingDetails(globalUser.id, meetingId);
      
      if (meetingDetails) {
        setMeetingDetails(meetingDetails);
        
        const signature = await getSignature(meetingDetails.id, 1);
        const { zak } = await getZak(globalUser.id);
        
        setSignature(signature);
        
        await startMeeting(
          signature.signature,
          signature.sdkKey,
          meetingDetails.id,
          meetingDetails.password,
          globalUser.name || "Sayso User",
          globalUser.email,
          zak
        );
      } else {
        console.error('❌ [ZoomClient] No meeting details received');
        setError("No meeting details found. Please check if the meeting exists and you have access to it.");
      }
    } catch (error) {
      console.error("❌ [ZoomClient] Error fetching meeting details:", error);
      console.error("❌ [ZoomClient] Error response:", error.response?.data);
      console.error("❌ [ZoomClient] Error status:", error.response?.status);
      
      if (error.response?.status === 500) {
        setError("Backend error: Unable to fetch meeting details. Please check if your Zoom account is properly connected.");
      } else if (error.response?.status === 401) {
        setError("Authentication error: Please reconnect your Zoom account.");
      } else if (error.response?.status === 404) {
        setError("Meeting not found: The meeting ID might be invalid or you don't have access to it.");
      } else {
        setError(`Failed to start meeting: ${error.message || 'Unknown error'}`);
      }
    } finally {
      setIsLoading(false);
    }
  };

  const handleManualPermission = async () => {
    try {
      const granted = await requestMediaPermissions();
      setPermissionStatus(granted ? 'granted' : 'denied');
      
      if (granted) {
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
      handleManualPermission();
    }
  }, [meetingId, globalUser]);

  return (
    <div>
      {isLoading && (
        <div className="flex items-center justify-center h-screen">
          <div className="text-center">
            <div className="animate-spin rounded-full h-12 w-12 border-b-2 border-blue-500 mx-auto mb-4"></div>
            <p className="text-lg text-gray-600">Loading Zoom meeting...</p>
          </div>
        </div>
      )}
      
      {error && (
        <div className="flex items-center justify-center h-screen">
          <div className="text-center max-w-md mx-auto p-6 bg-red-50 border border-red-200 rounded-lg">
            <div className="text-red-500 text-4xl mb-4">⚠️</div>
            <h3 className="text-lg font-semibold text-red-800 mb-2">Meeting Error</h3>
            <p className="text-red-600 mb-4">{error}</p>
            <button 
              onClick={() => window.history.back()} 
              className="px-4 py-2 bg-red-600 text-white rounded hover:bg-red-700"
            >
              Go Back
            </button>
          </div>
        </div>
      )}
      <div className="flex items-center justify-center h-screen governmenthooka">
        <div id="zmmtg-root" style={{
          display: 'none',
          position: 'relative',
          zIndex: 9998,
          background: 'white',
          minWidth: '800px'
        }}></div>
      </div>
      
      {/* {signature && meetingDetails && !isLoading && !error && (
        <div className="flex items-center justify-center h-screen governmenthooka">
          <div id="zmmtg-root" style={{
            display: 'block',
            position: 'relative',
            zIndex: 9998,
            background: 'white',
            minWidth: '800px'
          }}></div>
        </div>
      )} */}
    </div>
  );
};