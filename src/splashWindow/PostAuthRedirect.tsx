import SaysoLoader from "@/components/SaysoLoader";
import { useAuth } from "@/context/AuthContext";
import { Account } from "@/types/user";
import { useEffect } from "react";
import { useNavigate } from "react-router-dom";
import getCoachSettings from "@/appSettingsWindow/services/cue/getCoachSettings";

function shouldShowOnboarding(globalUser: Account): boolean {
    if (!globalUser.subscription_plan_id) return false;
    if (globalUser.onboarding_status === 'complete' || globalUser.onboarding_status === 'dismissed') return false;
    const remindAfter = localStorage.getItem('onboarding_remind_after');
    if (remindAfter && Date.now() < parseInt(remindAfter)) return false;
    return true;
}

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
                return;
            }

            window.electron?.ipcRenderer?.send('splash-login-success');

            if (globalUser && shouldShowOnboarding(globalUser)) {
                window.electron?.ipcRenderer?.send('open-onboarding-window');
            }
        }

        checkAndProceed();
    }, [loading, user, globalUser]);

    return <SaysoLoader />;
}
