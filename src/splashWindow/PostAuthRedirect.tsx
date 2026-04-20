import SaysoLoader from "@/components/SaysoLoader";
import { useAuth } from "@/context/AuthContext";
import { useEffect } from "react";
import { useNavigate } from "react-router-dom";
import getCoachSettings from "@/appSettingsWindow/services/cue/getCoachSettings";

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
            const [permissionsStatus, coachSettings] = await Promise.allSettled([
                window.electron?.permissions?.check(),
                getCoachSettings(),
            ]);

            if (coachSettings.status === 'fulfilled') {
                window.electron?.ipcRenderer?.send('set-font-size', coachSettings.value.font_size);
            }

            const mic = permissionsStatus.status === 'fulfilled' ? permissionsStatus.value?.mic : false;
            if (!mic) {
                navigate('/permissions', { replace: true });
            } else {
                window.electron?.ipcRenderer?.send('splash-login-success');
            }
        }

        checkAndProceed();
    }, [loading, user, globalUser]);

    return <SaysoLoader />;
}
