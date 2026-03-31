import { useState } from 'react';
import { useNavigate } from 'react-router-dom';
import LoginLayout from '@/components/layouts/LoginLayout';
import LoginBtn from '@/components/LoginBtn';
import { LuMic, LuMonitor } from 'react-icons/lu';
import './styles.css';

type PermissionsState = 'idle' | 'loading' | 'needs-system-settings';

export default function Permissions() {
    const navigate = useNavigate();
    const [state, setState] = useState<PermissionsState>('idle');

    async function handleAllow() {
        console.log(state,'state')
        if (state === 'needs-system-settings') {
            return;
        }

        setState('loading');
        const result = await window.electron?.permissions?.requestAll();
        console.log(result,'res')
        if (result?.mic && result?.screen) {
            localStorage.setItem('coachPermissionsGranted', 'true');
            navigate('/', { replace: true });
        } else if (!result?.mic) {
            setState('needs-system-settings');
        } else {
            localStorage.setItem('coachPermissionsGranted', 'true');
            navigate('/', { replace: true });
        }
    }

    if (state === 'needs-system-settings') {
        return (
            <LoginLayout
                title="Permissions required"
                description="Please enable Microphone and Screen & System Audio Recording in System Settings → Privacy & Security, then relaunch the app."
            >
                <div className="permissions-actions">
                    <LoginBtn text="Got it" onClick={() => window.electron?.ipcRenderer?.send('splash-login-success')} />
                </div>
            </LoginLayout>
        );
    }

    return (
        <LoginLayout
            title="One more step"
            description="Sayso needs a couple of permissions to coach you during calls. We only use these when you start a session."
        >
            <div className="permissions-items">
                <div className="permissions-item">
                    <LuMic className="permissions-item-icon" />
                    <div>
                        <p className="permissions-item-title">Microphone</p>
                        <p className="permissions-item-desc">To hear both sides of your call</p>
                    </div>
                </div>
                <div className="permissions-item">
                    <LuMonitor className="permissions-item-icon" />
                    <div>
                        <p className="permissions-item-title">Screen & System Audio</p>
                        <p className="permissions-item-desc">To capture audio from your speakers</p>
                    </div>
                </div>
            </div>
            <div className="permissions-actions">
                <LoginBtn text="Allow" onClick={handleAllow} isLoading={state === 'loading'} isDisabled={state === 'loading'} />
            </div>
        </LoginLayout>
    );
}
