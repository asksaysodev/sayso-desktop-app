import { useEffect, useState, useRef } from "react";
import { useNavigate, useParams } from "react-router-dom";
import { useSalesCoach } from '../hooks/useSalesCoach';

export default function PostCall() {
    const { meetingId, prospectId } = useParams();
    const [seconds, setSeconds] = useState(5);
    const { handleStopLiveCoach } = useSalesCoach();
    const navigate = useNavigate();
    const hasProcessedCall = useRef(false);

    const handleGoBack = () => {
        navigate('/');
    }

    useEffect(() => {
        const interval = setInterval(() => {
            setSeconds(prevSeconds => prevSeconds - 1);
        }, 1000);
        return () => {
            clearInterval(interval);
        };
    }, []);

    useEffect(() => {
        const timeout = setTimeout(() => {
            handleGoBack();
        }, 5000);
        return () => {
            clearTimeout(timeout);
        };
    }, []);

    useEffect(() => {
        let isMounted = true;
        
        const processCallEnd = async () => {
            
            if (hasProcessedCall.current) {
                return;
            }
            
            if (meetingId && prospectId && isMounted) {
                hasProcessedCall.current = true;
                try {
                    await handleStopLiveCoach(meetingId, prospectId);
                } catch (error) {
                    console.error('❌ Error in handleStopLiveCoach:', error);
                }
            } else if (isMounted) {
                handleGoBack();
            }
        };

        processCallEnd();

        return () => {
            isMounted = false;
        };
    }, [handleStopLiveCoach, handleGoBack, meetingId, prospectId]);

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