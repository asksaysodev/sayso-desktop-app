import { useEffect, useState } from "react";
import { useNavigate } from "react-router-dom";

export default function ZoomCallback() {

    const [seconds, setSeconds] = useState(5);
    const [connected, setConnected] = useState(false);

    const queryParams = new URLSearchParams(window.location.search);

    const navigate = useNavigate();

    const handleGoBack = () => {
        navigate('/');
    }

    useEffect(() => {
        const interval = setInterval(() => {
            setSeconds(prevSeconds => prevSeconds - 1);
        }, 1000);
        return () => clearInterval(interval);
    }, []);

    useEffect(() => {
        setTimeout(() => {
            handleGoBack();
        }, 5000);
    }, []);

    useEffect(() => {
        // Handle URL parameters (for web version)
        const connected = queryParams.get('connected');
        if (connected === 'true') {
            setConnected(true);
        } else if (connected === 'false') {
            setConnected(false);
        }
    }, [queryParams]);

    useEffect(() => {
        // Handle protocol callback (for Electron app)
        if (window.electron?.ipcRenderer) {
            const cleanup = window.electron.ipcRenderer.on('zoom-callback', (data) => {
        
                setConnected(data.connected);
            });
            
            return cleanup; // Cleanup function
        }
    }, []);

    return (
        <div className="zoom-success" style={{ display: 'flex', flexDirection: 'column', alignItems: 'center', justifyContent: 'center', height: '100vh' }}>
            <h1 style={{ fontSize: '1.5rem', fontWeight: 'bold', marginBottom: '50px' }}>{connected ? 'Account Connected!' : 'Your Zoom account was not connected.'}</h1>
            {connected &&
                <div style={{  height: '300px' }}>
                    <img src='/assets/zoom_success.png' alt="Zoom Success" style={{ width: '100%', height: '100%' }} />
                </div>
            }
            <p style={{ fontSize: '1.2rem', marginTop: '50px' , opacity: 1, cursor: 'pointer'}} onClick={handleGoBack}>Go back to dashboard</p>
            <p style={{ fontSize: '1.2rem', marginTop: '20px' , opacity: 0.6}}>Redirecting to Sayso in {seconds} seconds...</p>
        </div>
    );
}