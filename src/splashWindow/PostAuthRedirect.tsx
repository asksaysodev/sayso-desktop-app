import SaysoLoader from "@/components/SaysoLoader";
import { useAuth } from "@/context/AuthContext";
import { useEffect } from "react";
import { useNavigate } from "react-router-dom";

// Default splash route. Handles two scenarios:
// 1. Unauthenticated cold start → /login
// 2. Authenticated but no permissions flag (e.g. token restored after SCK relaunch before flag was written) → /permissions
export default function PostAuthRedirect() {
    const { loading, user } = useAuth();
    const navigate = useNavigate();

    useEffect(() => {
        if (loading) return;

        if (!user) {
            navigate('/login', { replace: true });
            return;
        }

        const checkFlag = async () => {
            const flagSet = await window.electron?.ipcRenderer?.invoke('permissions-get-flag') as boolean | undefined;
            if (flagSet) {
                // Authenticated + permissions complete → nothing to show; tell main to close splash.
                window.electron?.ipcRenderer?.send('splash-login-success');
            } else {
                navigate('/permissions', { replace: true });
            }
        };

        checkFlag();
    }, [loading, user]);

    return <SaysoLoader />;
}
