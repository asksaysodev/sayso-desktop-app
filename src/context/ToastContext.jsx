import { createContext, useContext, useState, useRef } from 'react';
import Toast from '../components/Toast';

const ToastContext = createContext();

export const useToast = () => {
    const context = useContext(ToastContext);
    if (!context) {
        throw new Error('useToast must be used within a ToastProvider');
    }
    return context;
};

export const ToastProvider = ({ children }) => {

    const [toast, setToast] = useState(null);
    const autoHideTimeoutRef = useRef(null);

    const showToast = (status, text) => {
        // Clear any existing timeout
        if (autoHideTimeoutRef.current) {
            clearTimeout(autoHideTimeoutRef.current);
        }
        
        setToast({ status, text });
        
        autoHideTimeoutRef.current = setTimeout(() => {
            setToast(prev => prev ? { ...prev, autoClose: true } : null);
        }, 1500);
    };

    const hideToast = () => {
        if (autoHideTimeoutRef.current) {
            clearTimeout(autoHideTimeoutRef.current);
        }
        setToast(null);
    };

    return (
        <ToastContext.Provider value={{ showToast, hideToast }}>
            {children}
            {toast && (
                <Toast 
                    status={toast.status} 
                    text={toast.text} 
                    onClose={hideToast}
                    autoClose={toast.autoClose}
                />
            )}
        </ToastContext.Provider>
    );
};
