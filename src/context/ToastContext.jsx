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
        
        // Auto-hide after 2 seconds
        autoHideTimeoutRef.current = setTimeout(() => {
            // Instead of directly setting toast to null, we need to trigger the close animation
            // We'll pass a special prop to the Toast component to handle auto-close
            setToast(prev => prev ? { ...prev, autoClose: true } : null);
        }, 2000);
    };

    const hideToast = () => {
        // Clear the auto-hide timeout when manually closing
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
