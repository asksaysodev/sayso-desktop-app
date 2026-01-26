import { useState, useEffect, useCallback } from 'react';
import { supabase } from '../config/supabase';
import '../styles/StreamingTest.css';
import useCue from '../coachWindow/hooks/useCue';

export default function StreamingTest() {
  const [status, setStatus] = useState({
    user: 'disconnected',
    prospect: 'disconnected'
  });
  const [logs, setLogs] = useState([]);
  const [error, setError] = useState(null);
  const [token, setToken] = useState(null);

  const { 
    handleStartCue, 
    handleStopCue, 
    isCueActive, 
    sessionId, 
    isLoading 
  } = useCue();

  // Add log helper
  const addLog = useCallback((message, type = 'info') => {
    const timestamp = new Date().toLocaleTimeString();
    const logEntry = { timestamp, message, type };
    setLogs(prev => [...prev, logEntry]);
    console.log(`[${timestamp}] ${message}`);
  }, []);


  // Get token on mount
  useEffect(() => {
    const getToken = async () => {
      try {
        const { data: { session } } = await supabase.auth.getSession();
        if (session?.access_token) {
          setToken(session.access_token);
          addLog('✅ Token retrieved successfully', 'success');
        } else {
          addLog('❌ No session found. Please log in.', 'error');
        }
      } catch (err) {
        addLog(`❌ Error getting token: ${err.message}`, 'error');
        setError(err.message);
      }
    };
    getToken();
  }, [addLog]);

  // Listen for cue events
  useEffect(() => {
    if (!window.electron?.cue) {
      addLog('⚠️ Cue API not available', 'warning');
      return;
    }

    // Listen for status updates (when streams connect)
    const unsubscribeStatus = window.electron.cue.onStatus((data) => {
      addLog(`📡 Cue status update: ${JSON.stringify(data)}`, 'info');
      setStatus(prev => ({ ...prev, ...data }));
    });

    // Listen for errors
    const unsubscribeError = window.electron.cue.onError((data) => {
      addLog(`❌ Cue error [${data.stream}]: ${data.error}`, 'error');
      setError(`${data.stream}: ${data.error}`);
    });

    return () => {
      unsubscribeStatus();
      unsubscribeError();
    };
  }, [addLog]);

  // Start cue streaming
  const handleStart = useCallback(async () => {
    if (!token) {
      addLog('❌ No token available. Please log in.', 'error');
      setError('No token available');
      return;
    }

    if (!window.electron?.cue) {
      addLog('❌ Cue API not available', 'error');
      setError('Cue API not available');
      return;
    }

    try {
      addLog('🚀 Starting cue streaming...', 'info');
      setError(null);
      
      await handleStartCue();
      
      addLog('✅ Cue started successfully!', 'success');
    } catch (err) {
      addLog(`❌ Failed to start cue: ${err.message}`, 'error');
      setError(err.message);
    }
  }, [token, addLog, handleStartCue]);

  // Stop cue streaming
  const handleStop = useCallback(async () => {
    if (!window.electron?.cue) {
      addLog('❌ Cue API not available', 'error');
      return;
    }

    if (!sessionId) {
      addLog('❌ No active session to stop', 'error');
      return;
    }

    try {
      addLog('🛑 Stopping cue streaming...', 'info');
      
      await handleStopCue(sessionId);
      
      setStatus({
        user: 'disconnected',
        prospect: 'disconnected'
      });
      addLog('✅ Cue stopped successfully', 'success');
    } catch (err) {
      addLog(`❌ Failed to stop cue: ${err.message}`, 'error');
      setError(err.message);
    }
  }, [addLog, handleStopCue, sessionId]);

  // Get status (shows current hook state)
  const handleGetStatus = useCallback(() => {
    const currentStatus = {
      isCueActive,
      sessionId,
      isLoading,
      streamStatus: status
    };
    addLog(`📊 Current Status: ${JSON.stringify(currentStatus, null, 2)}`, 'info');
  }, [addLog, isCueActive, sessionId, isLoading, status]);

  // Clear logs
  const handleClearLogs = useCallback(() => {
    setLogs([]);
    addLog('🧹 Logs cleared', 'info');
  }, [addLog]);

  return (
    <div className="streaming-test">
      <div className="streaming-test-header">
        <h2>🎤 Audio Streaming Test</h2>
        <div className="status-indicators">
          <div className={`status-indicator ${status.user}`}>
            User: {status.user}
          </div>
          <div className={`status-indicator ${status.prospect}`}>
            Prospect: {status.prospect}
          </div>
          <div className={`status-indicator ${isCueActive ? 'streaming' : 'idle'}`}>
            {isCueActive ? '🟢 Cue Active' : '⚪ Idle'}
          </div>
          {isLoading && (
            <div className="status-indicator loading">
              ⏳ Loading...
            </div>
          )}
        </div>
      </div>

      {sessionId && (
        <div className="session-info">
          <strong>Session ID:</strong> {sessionId}
        </div>
      )}

      {error && (
        <div className="error-message">
          ⚠️ Error: {error}
        </div>
      )}

      <div className="controls">
        <button 
          onClick={handleStart} 
          disabled={isCueActive || isLoading || !token}
          className="btn-start"
        >
          ▶️ Start Cue
        </button>
        <button 
          onClick={handleStop} 
          disabled={!isCueActive || isLoading}
          className="btn-stop"
        >
          ⏹️ Stop Cue
        </button>
        <button 
          onClick={handleGetStatus}
          className="btn-status"
        >
          📊 Get Status
        </button>
        <button 
          onClick={handleClearLogs}
          className="btn-clear"
        >
          🧹 Clear Logs
        </button>
      </div>

      <div className="logs-container">
        <div className="logs-header">
          <h3>Logs ({logs.length})</h3>
        </div>
        <div className="logs">
          {logs.length === 0 ? (
            <div className="log-entry empty">No logs yet...</div>
          ) : (
            logs.map((log, index) => (
              <div key={index} className={`log-entry ${log.type}`}>
                <span className="log-time">[{log.timestamp}]</span>
                <span className="log-message">{log.message}</span>
              </div>
            ))
          )}
        </div>
      </div>
    </div>
  );
}

