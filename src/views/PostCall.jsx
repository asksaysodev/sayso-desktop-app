import { useEffect, useState, useRef } from "react";
import { useNavigate, useParams } from "react-router-dom";
import { useSalesCoach } from '../hooks/useSalesCoach';

export default function PostCall() {
    const { meetingId, prospectId, sessionId } = useParams();
    const [seconds, setSeconds] = useState(5);
    const { handleStopLiveCoach } = useSalesCoach();
    const navigate = useNavigate();
    const hasProcessedCall = useRef(false);
    const hasNavigated = useRef(false);

    const handleGoBack = () => {
        if (!hasNavigated.current) {
            hasNavigated.current = true;
            navigate('/');
        }
    }

    useEffect(() => {
        let isMounted = true;
        
        const processCallEnd = async () => {
            if (hasProcessedCall.current) {
                return;
            }
            
            // Check if meeting already ended (reload happened)
            if (sessionStorage.getItem('meetingEnded')) {
                sessionStorage.removeItem('meetingEnded');
                // Don't process again, just navigate
                setTimeout(() => {
                    if (isMounted) handleGoBack();
                }, 2000); // Shorter delay since reload already happened
                return;
            }
            
            if (meetingId && prospectId && sessionId && isMounted) {
                hasProcessedCall.current = true;
                try {
                    await handleStopLiveCoach(meetingId, prospectId, sessionId);
                    // Don't reload here - let the component handle navigation
                } catch (error) {
                    console.error('❌ Error in handleStopLiveCoach:', error);
                    if (isMounted) handleGoBack();
                }
            } else if (isMounted) {
                handleGoBack();
            }
        };

        // Start countdown timer
        const interval = setInterval(() => {
            if (isMounted) {
                setSeconds(prevSeconds => {
                    if (prevSeconds <= 1) {
                        clearInterval(interval);
                        handleGoBack();
                        return 0;
                    }
                    return prevSeconds - 1;
                });
            }
        }, 1000);

        // Process call end immediately
        processCallEnd();

        return () => {
            isMounted = false;
            clearInterval(interval);
        };
    }, [handleStopLiveCoach, handleGoBack, meetingId, prospectId, sessionId]);

    return (
        <div className="zoom-success" style={{ display: 'flex', flexDirection: 'column', alignItems: 'center', justifyContent: 'center', height: '100vh' }}>
            <h1 style={{ fontSize: '1.5rem', fontWeight: 'bold', marginBottom: '50px' }}>Meeting and Coach Session Completed!</h1>
            <div style={{  height: '300px' }}>
                <img src='./assets/loader-logo.png' alt="Zoom Success" style={{ width: '100%', height: '100%' }} />
            </div>
            <p style={{ fontSize: '1.2rem', marginTop: '50px' , opacity: 1, cursor: 'pointer'}} onClick={handleGoBack}>Go back to Dashboard</p>
            <p style={{ fontSize: '1.2rem', marginTop: '20px' , opacity: 0.6}}>Redirecting to Sayso in {seconds} seconds...</p>
        </div>
    );
}