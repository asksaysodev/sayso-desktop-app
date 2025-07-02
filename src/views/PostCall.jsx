import { useEffect, useState, useRef } from "react";
import { useNavigate, useParams, useLocation } from "react-router-dom";
// import { useSalesCoachContext } from '../context/SalesCoachContext';
import { useSalesCoach } from '../hooks/useSalesCoach';

export default function PostCall() {
    const { meetingId, prospectId } = useParams();
    const location = useLocation();
    const [seconds, setSeconds] = useState(5);
    const [success, setSuccess] = useState(false);
    const { handleStopLiveCoach } = useSalesCoach();
    const navigate = useNavigate();
    const hasProcessedCall = useRef(false);

    useEffect(() => {
        const params = new URLSearchParams(location.search);
        const success = params.get('success');
        console.log('🟢 PostCall mounted!');
        console.log('🟢 window.location.href:', window.location.href);
        console.log('🟢 meetingId:', meetingId);
        console.log('🟢 prospectId:', prospectId);
        console.log('🟢 success:', success);
    }, [meetingId, prospectId, location.search]);

    const handleGoBack = () => {
        // console.log('🔙 handleGoBack called');
        navigate('/');
    }

    useEffect(() => {
        // console.log('⏱️ Timer effect mounted');
        const interval = setInterval(() => {
            setSeconds(prevSeconds => prevSeconds - 1);
        }, 1000);
        return () => {
            // console.log('⏱️ Timer effect cleanup');
            clearInterval(interval);
        };
    }, []);

    useEffect(() => {
        // console.log('🕒 Redirect effect mounted');
        const timeout = setTimeout(() => {
            handleGoBack();
        }, 5000);
        return () => {
            // console.log('🕒 Redirect effect cleanup');
            clearTimeout(timeout);
        };
    }, []);

    useEffect(() => {
        // console.log('🎯 Process call effect mounted');
        let isMounted = true;
        
        const processCallEnd = async () => {
            // console.log('🔄 processCall End started, hasProcessedCall:', hasProcessedCall.current);
            
            if (hasProcessedCall.current) {
                // console.log('⏭️ Skipping processCallEnd - already processed');
                return;
            }
            
            const queryParams = new URLSearchParams(window.location.search);
            const sessionSuccess = queryParams.get('success');
            
            // console.log('📊 Session status:', { sessionSuccess, meetingId, isMounted });
            
            if (sessionSuccess === 'true' && isMounted) {
                // console.log('✅ Setting success state and processing call');
                setSuccess(true);
                hasProcessedCall.current = true;
                try {
                    // console.log('📡 Calling handleStopLiveCoach...');
                    await handleStopLiveCoach(meetingId, prospectId);
                    // console.log('✅ handleStopLiveCoach completed successfully');
                } catch (error) {
                    console.error('❌ Error in handleStopLiveCoach:', error);
                }
            } else if (isMounted) {
                // console.log('❌ Session not successful, redirecting...');
                handleGoBack();
            }
        };

        processCallEnd();

        return () => {
            // console.log('🎯 Process call effect cleanup');
            isMounted = false;
        };
    }, [handleStopLiveCoach, handleGoBack]);

    return (
        <div className="zoom-success" style={{ display: 'flex', flexDirection: 'column', alignItems: 'center', justifyContent: 'center', height: '100vh' }}>
            <h1 style={{ fontSize: '1.5rem', fontWeight: 'bold', marginBottom: '50px' }}>{success ? 'Meeting and Coach Session Completed!' : 'Coach Session Failed.'}</h1>
            {success &&
                <div style={{  height: '300px' }}>
                    <img src='/assets/loader-logo.png' alt="Zoom Success" style={{ width: '100%', height: '100%' }} />
                </div>
            }
            <p style={{ fontSize: '1.2rem', marginTop: '50px' , opacity: 1, cursor: 'pointer'}} onClick={handleGoBack}>Go back to Dashboard</p>
            <p style={{ fontSize: '1.2rem', marginTop: '20px' , opacity: 0.6}}>Redirecting to Sayso in {seconds} seconds...</p>
        </div>
    );
}