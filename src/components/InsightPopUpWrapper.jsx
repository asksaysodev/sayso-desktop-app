import { useState, useEffect } from 'react';
import { useSalesCoachContext } from '../context/SalesCoachContext';
import InsightPopUp from './InsightPopUp';

export default function InsightPopUpWrapper() {

    const [displayMessage, setDisplayMessage] = useState(null);
    const [isClosing, setIsClosing] = useState(false);

    const { currentInsight } = useSalesCoachContext();

    const handleClose = (isIceBreaker) => {

        setTimeout(() => {
            setIsClosing(true);
            setTimeout(() => {
                setDisplayMessage(null);
                
            }, 200);
        }, isIceBreaker ? 15000 : 8000);

    }

    useEffect(() => {
        if (currentInsight && currentInsight.message && currentInsight.message !== '') {
            
            setIsClosing(false);
            if(currentInsight.isIceBreaker) {
                setTimeout(() => {
                    setDisplayMessage(currentInsight.message);
                    handleClose(true)
                }, 3000);
            } else {
                setDisplayMessage(currentInsight.message);
                handleClose(false)
            }
        } 
    }, [currentInsight]);

    return (
        <>
            {displayMessage && (
                <InsightPopUp 
                    message={displayMessage} 
                    isClosing={isClosing} 
                />
            )}
        </>
    );
} 