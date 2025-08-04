import React, { useState } from 'react';
import { runSecurityTests, testLocalStorageEncryption } from '../utils/securityTest';

const SecurityTestButton = () => {
  const [isRunning, setIsRunning] = useState(false);
  const [results, setResults] = useState(null);

  const handleRunTests = async () => {
    setIsRunning(true);
    setResults(null);
    
    try {
      console.log('🚀 Starting security tests...');
      const testResults = await runSecurityTests();
      setResults(testResults);
    } catch (error) {
      console.error('Test error:', error);
    } finally {
      setIsRunning(false);
    }
  };

  const handleTestEncryption = async () => {
    console.log('🔐 Testing localStorage encryption...');
    await testLocalStorageEncryption();
  };

  return (
    <div style={{ 
      position: 'fixed', 
      bottom: '20px', 
      right: '20px', 
      zIndex: 9999,
      background: '#1a1a1a',
      padding: '15px',
      borderRadius: '8px',
      border: '1px solid #333',
      boxShadow: '0 4px 12px rgba(0,0,0,0.3)'
    }}>
      <h4 style={{ margin: '0 0 10px 0', color: '#fff' }}>🔐 Security Tests</h4>
      
      <button 
        onClick={handleRunTests}
        disabled={isRunning}
        style={{
          background: isRunning ? '#666' : '#007bff',
          color: 'white',
          border: 'none',
          padding: '8px 12px',
          borderRadius: '4px',
          cursor: isRunning ? 'not-allowed' : 'pointer',
          marginRight: '8px',
          fontSize: '12px'
        }}
      >
        {isRunning ? 'Running...' : 'Run Tests'}
      </button>
      
      <button 
        onClick={handleTestEncryption}
        style={{
          background: '#28a745',
          color: 'white',
          border: 'none',
          padding: '8px 12px',
          borderRadius: '4px',
          cursor: 'pointer',
          fontSize: '12px'
        }}
      >
        Test Encryption
      </button>
      
      {results && (
        <div style={{ marginTop: '10px', fontSize: '11px' }}>
          <div style={{ color: '#fff', marginBottom: '5px' }}>Results:</div>
          {Object.entries(results).map(([test, passed]) => (
            <div key={test} style={{ color: passed ? '#28a745' : '#dc3545' }}>
              {passed ? '✅' : '❌'} {test}
            </div>
          ))}
        </div>
      )}
      
      <div style={{ 
        fontSize: '10px', 
        color: '#999', 
        marginTop: '8px',
        fontStyle: 'italic'
      }}>
        Check console for details
      </div>
    </div>
  );
};

export default SecurityTestButton; 