import React, { useState, useEffect } from 'react';
import './NativeAudioTest.css';

const NativeAudioTest = () => {
  const [devices, setDevices] = useState([]);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState(null);
  const [isInitialized, setIsInitialized] = useState(false);
  const [isCapturing, setIsCapturing] = useState(false);
  const [captureStatus, setCaptureStatus] = useState('');

  const initializeNativeAudio = async () => {
    try {
      setLoading(true);
      setError(null);
      
      console.log('🎤 [TEST] Initializing native audio...');
      const result = await window.electron.nativeAudio.initialize();
      
      if (result.success) {
        setIsInitialized(true);
        console.log('🎤 [TEST] ✅ Native audio initialized successfully');
      } else {
        throw new Error(result.error || 'Failed to initialize');
      }
    } catch (error) {
      console.error('🎤 [TEST] ❌ Failed to initialize:', error);
      setError(error.message);
    } finally {
      setLoading(false);
    }
  };

  const listDevices = async () => {
    try {
      setLoading(true);
      setError(null);
      
      console.log('🎤 [TEST] Listing devices...');
      const result = await window.electron.nativeAudio.listDevices();
      
      if (result.success) {
        setDevices(result.devices);
        console.log('🎤 [TEST] ✅ Devices listed:', result.devices);
      } else {
        throw new Error(result.error || 'Failed to list devices');
      }
    } catch (error) {
      console.error('🎤 [TEST] ❌ Failed to list devices:', error);
      setError(error.message);
    } finally {
      setLoading(false);
    }
  };

  const createTestDevice = async () => {
    try {
      setLoading(true);
      setError(null);
      
      // Find BlackHole device
      const blackHoleDevice = devices.find(d => d.name.includes('BlackHole'));
      if (!blackHoleDevice) {
        throw new Error('BlackHole device not found');
      }
      
      // Find a speaker device
      const speakerDevice = devices.find(d => 
        d.name.includes('Speaker') || 
        d.name.includes('MacBook') || 
        d.name.includes('BoomAudio')
      );
      if (!speakerDevice) {
        throw new Error('Speaker device not found');
      }
      
      console.log('🎤 [TEST] Creating test device...');
      const result = await window.electron.nativeAudio.createDevice('Sayso Speaker Test', [
        blackHoleDevice.name,
        speakerDevice.name
      ]);
      
      if (result.success) {
        console.log('🎤 [TEST] ✅ Test device created with ID:', result.deviceId);
        alert(`Test device created successfully with ID: ${result.deviceId}`);
      } else {
        throw new Error(result.error || 'Failed to create device');
      }
    } catch (error) {
      console.error('🎤 [TEST] ❌ Failed to create device:', error);
      setError(error.message);
    } finally {
      setLoading(false);
    }
  };

  const startSystemAudioCapture = async () => {
    try {
      setLoading(true);
      setError(null);
      
      console.log('🎤 [TEST] Starting system audio capture...');
      const result = await window.electron.nativeAudio.startCapture();
      
      if (result.success) {
        setIsCapturing(true);
        setCaptureStatus('System audio capture started!');
        console.log('🎤 [TEST] ✅ System audio capture started');
      } else {
        throw new Error(result.error || 'Failed to start capture');
      }
    } catch (error) {
      console.error('🎤 [TEST] ❌ Failed to start capture:', error);
      setError(error.message);
    } finally {
      setLoading(false);
    }
  };

  const stopSystemAudioCapture = async () => {
    try {
      setLoading(true);
      setError(null);
      
      console.log('🎤 [TEST] Stopping system audio capture...');
      const result = await window.electron.nativeAudio.stopCapture();
      
      if (result.success) {
        setIsCapturing(false);
        setCaptureStatus('System audio capture stopped!');
        console.log('🎤 [TEST] ✅ System audio capture stopped');
      } else {
        throw new Error(result.error || 'Failed to stop capture');
      }
    } catch (error) {
      console.error('🎤 [TEST] ❌ Failed to stop capture:', error);
      setError(error.message);
    } finally {
      setLoading(false);
    }
  };

  const checkCaptureStatus = async () => {
    try {
      const result = await window.electron.nativeAudio.isCapturing();
      if (result.success) {
        setIsCapturing(result.isCapturing);
        setCaptureStatus(result.isCapturing ? 'Capturing active' : 'Not capturing');
      }
    } catch (error) {
      console.error('🎤 [TEST] ❌ Failed to check status:', error);
    }
  };

  return (
    <div className="native-audio-test">
      <h2>🎤 Native Audio Module Test</h2>
      
      <div className="test-controls">
        <button 
          onClick={initializeNativeAudio} 
          disabled={loading || isInitialized}
          className="test-button"
        >
          {isInitialized ? '✅ Initialized' : 'Initialize Native Audio'}
        </button>
        
        <button 
          onClick={listDevices} 
          disabled={loading || !isInitialized}
          className="test-button"
        >
          List Output Devices
        </button>
        
        <button 
          onClick={createTestDevice} 
          disabled={loading || !isInitialized || devices.length === 0}
          className="test-button"
        >
          Create Test Device
        </button>
        
        <button 
          onClick={startSystemAudioCapture} 
          disabled={loading || !isInitialized || isCapturing}
          className="test-button"
        >
          Start System Audio Capture
        </button>
        
        <button 
          onClick={stopSystemAudioCapture} 
          disabled={loading || !isInitialized || !isCapturing}
          className="test-button"
        >
          Stop System Audio Capture
        </button>
        
        <button 
          onClick={checkCaptureStatus} 
          disabled={loading || !isInitialized}
          className="test-button"
        >
          Check Capture Status
        </button>
      </div>
      
      {loading && <div className="loading">Loading...</div>}
      
      {error && (
        <div className="error">
          <strong>Error:</strong> {error}
        </div>
      )}
      
      {captureStatus && (
        <div className="capture-status">
          <strong>Capture Status:</strong> {captureStatus}
        </div>
      )}
      
      {devices.length > 0 && (
        <div className="devices-list">
          <h3>Output Devices ({devices.length})</h3>
          <ul>
            {devices.map((device, index) => (
              <li key={index}>
                <strong>ID:</strong> {device.id} | <strong>Name:</strong> {device.name}
              </li>
            ))}
          </ul>
        </div>
      )}
    </div>
  );
};

export default NativeAudioTest;
