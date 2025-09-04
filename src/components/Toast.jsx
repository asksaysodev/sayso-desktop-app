import { useState, useEffect } from 'react';
// import { LuCheckCircle } from 'react-icons/lu';
import { VscError } from 'react-icons/vsc';
import { FaRegCircleCheck,  } from 'react-icons/fa6';
import { MdOutlineErrorOutline } from 'react-icons/md';

import '../styles/Toast.css';

export default function Toast({ status, text, onClose, autoClose = false }) {
    const [isVisible, setIsVisible] = useState(false);
    const [isClosing, setIsClosing] = useState(false);

    useEffect(() => {
        const timer = setTimeout(() => {
            setIsVisible(true);
        }, 10);
        
        return () => clearTimeout(timer);
    }, []);

    // Handle auto-close animation
    useEffect(() => {
        if (autoClose) {
            setIsClosing(true);
            setTimeout(() => {
                onClose();
            }, 300);
        }
    }, [autoClose, onClose]);

    const handleClose = () => {
        setIsClosing(true);
        setTimeout(() => {
            onClose();
        }, 300);
    };

    return (
        <div 
            className={`toast-main-container ${status} ${isVisible ? 'slide-in' : ''} ${isClosing ? 'slide-out' : ''}`}
            onClick={handleClose}
        >
            <div className="outline"></div>
            <div className={`toast-container ${status} `}>
                {status === 'success' && <FaRegCircleCheck/>}
                {status === 'error' && <VscError/>}
                {status === 'warning' && <MdOutlineErrorOutline/>}
                <span className="toast-text">{text}</span>
                
            </div>
        </div>
    )
}