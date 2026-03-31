import SaysoLoader from "@/components/SaysoLoader";
import { useAuth } from "@/context/AuthContext";
import { useEffect } from "react";
import { useNavigate } from "react-router-dom";

export default function PostAuthRedirect() {
    const { loading, user, globalUser } = useAuth();
    const navigate = useNavigate();

    useEffect(() => {
        if (loading) return;
        if (!user) {
            navigate('/login', { replace: true });
            return;
        }

        if (!globalUser) return;

        async function checkAndProceed() {
            const permissionsStatus = await window.electron?.permissions?.check();
            if (!permissionsStatus?.mic) {
                navigate('/permissions', { replace: true });
            } else {
                window.electron?.ipcRenderer?.send('splash-login-success');
            }
        }

        checkAndProceed();
    }, [loading, user, globalUser]);

    return <SaysoLoader />;
}
