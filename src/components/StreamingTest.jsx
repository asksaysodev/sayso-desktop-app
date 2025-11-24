import { useState, useEffect, useCallback } from 'react';
import { supabase } from '../config/supabase';
import '../styles/StreamingTest.css';
import useCue from '../coachWindow/hooks/useCue';

export default function StreamingTest() {
  const [isStreaming, setIsStreaming] = useState(false);
  const [status, setStatus] = useState({
    user: 'disconnected',
    prospect: 'disconnected',
    sessionId: null
  });
  const [logs, setLogs] = useState([]);
  const [error, setError] = useState(null);
  const [token, setToken] = useState(null);

  const { handleStartCue } = useCue();

  // Add log helper
  const addLog = useCallback((message, type = 'info') => {
    const timestamp = new Date().toLocaleTimeString();
    const logEntry = { timestamp, message, type };
    setLogs(prev => [...prev, logEntry]);
    console.log(`[${timestamp}] ${message}`);
  }, []);

  const testActualCue = async () => {
    try {
      const response = await handleStartCue();
      console.log('response', response);
    } catch (error) {
      console.error('Error testing actual cue:', error);
    }
  }

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

  // Listen for streaming events
  useEffect(() => {
    if (!window.electron?.streaming) {
      addLog('⚠️ Streaming API not available', 'warning');
      return;
    }

    // Listen for status updates
    const unsubscribeStatus = window.electron.streaming.onStatus((data) => {
      addLog(`📡 Status update: ${JSON.stringify(data)}`, 'info');
      setStatus(prev => ({ ...prev, ...data }));
    });

    // Listen for errors
    const unsubscribeError = window.electron.streaming.onError((data) => {
      addLog(`❌ Streaming error [${data.stream}]: ${data.error}`, 'error');
      setError(`${data.stream}: ${data.error}`);
    });

    return () => {
      unsubscribeStatus();
      unsubscribeError();
    };
  }, [addLog]);

  // Start streaming
  const handleStart = useCallback(async () => {
    if (!token) {
      addLog('❌ No token available. Please log in.', 'error');
      setError('No token available');
      return;
    }

    if (!window.electron?.streaming) {
      addLog('❌ Streaming API not available', 'error');
      setError('Streaming API not available');
      return;
    }

    try {
      addLog('🚀 Starting audio streaming...', 'info');
      setError(null);
      
      const result = await window.electron.streaming.start({ token });
      
      if (result.success) {
        setIsStreaming(true);
        setStatus(prev => ({ ...prev, sessionId: result.sessionId }));
        addLog(`✅ Streaming started successfully! SessionId: ${result.sessionId}`, 'success');
      } else {
        throw new Error(result.error || 'Failed to start streaming');
      }
    } catch (err) {
      addLog(`❌ Failed to start streaming: ${err.message}`, 'error');
      setError(err.message);
      setIsStreaming(false);
    }
  }, [token, addLog]);

  // Stop streaming
  const handleStop = useCallback(async () => {
    if (!window.electron?.streaming) {
      addLog('❌ Streaming API not available', 'error');
      return;
    }

    try {
      addLog('🛑 Stopping audio streaming...', 'info');
      
      const result = await window.electron.streaming.stop({ sendTermination: true });
      
      if (result.success) {
        setIsStreaming(false);
        setStatus({
          user: 'disconnected',
          prospect: 'disconnected',
          sessionId: null
        });
        addLog('✅ Streaming stopped successfully', 'success');
      } else {
        throw new Error(result.error || 'Failed to stop streaming');
      }
    } catch (err) {
      addLog(`❌ Failed to stop streaming: ${err.message}`, 'error');
      setError(err.message);
    }
  }, [addLog]);

  // Get status
  const handleGetStatus = useCallback(async () => {
    if (!window.electron?.streaming) {
      addLog('❌ Streaming API not available', 'error');
      return;
    }

    try {
      const currentStatus = await window.electron.streaming.getStatus();
      addLog(`📊 Status: ${JSON.stringify(currentStatus)}`, 'info');
      setStatus(prev => ({
        ...prev,
        user: currentStatus.userState || prev.user,
        prospect: currentStatus.prospectState || prev.prospect,
        sessionId: currentStatus.sessionId || prev.sessionId
      }));
      setIsStreaming(currentStatus.isStreaming || false);
    } catch (err) {
      addLog(`❌ Failed to get status: ${err.message}`, 'error');
    }
  }, [addLog]);

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
          <div className={`status-indicator ${isStreaming ? 'streaming' : 'idle'}`}>
            {isStreaming ? '🟢 Streaming' : '⚪ Idle'}
          </div>
        </div>
      </div>

      {status.sessionId && (
        <div className="session-info">
          <strong>Session ID:</strong> {status.sessionId}
        </div>
      )}

      {error && (
        <div className="error-message">
          ⚠️ Error: {error}
        </div>
      )}

      <div className="controls">
        <button 
          // onClick={handleStart} 
          onClick={() => testActualCue()}
          disabled={isStreaming || !token}
          className="btn-start"
        >
          ▶️ Start Streaming
        </button>
        <button 
          onClick={handleStop} 
          disabled={!isStreaming}
          className="btn-stop"
        >
          ⏹️ Stop Streaming
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

