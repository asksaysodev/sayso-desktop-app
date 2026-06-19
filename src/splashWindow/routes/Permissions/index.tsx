import { useEffect, useRef, useState } from 'react';
import { LuCheck, LuMic, LuLoader, LuHeadphones } from 'react-icons/lu';
import './styles.css';

type PermStatus = 'pending' | 'granted' | 'denied';

interface PermState {
    mic: PermStatus;
    screen: PermStatus;
}

const POLL_INTERVAL_MS = 1500;

export default function Permissions() {
    const [perms, setPerms] = useState<PermState>({ mic: 'pending', screen: 'pending' });
    const [micLoading, setMicLoading] = useState(false);
    const [screenOpened, setScreenOpened] = useState(false);
    const [completing, setCompleting] = useState(false);
    const pollRef = useRef<ReturnType<typeof setInterval> | null>(null);

    const bothGranted = perms.mic === 'granted' && perms.screen === 'granted';
    // CGPreflightScreenCaptureAccess caches false for the process lifetime after being denied at launch,
    // so we can't rely on the poll to confirm mid-session. Once the user has opened System Settings
    // and mic is already granted, surface "Quit and Reopen" directly.
    const canComplete = bothGranted || (perms.mic === 'granted' && screenOpened);

    // On mount, check current state so returning users see accurate status
    useEffect(() => {
        const init = async () => {
            const result = await window.electron?.ipcRenderer?.invoke('permissions-check') as
                { mic: boolean; screen: boolean } | undefined;
            if (result) {
                setPerms({
                    mic: result.mic ? 'granted' : 'pending',
                    screen: result.screen ? 'granted' : 'pending',
                });
            }
        };
        init();
    }, []);

    // Poll for screen recording grant once user has opened settings
    useEffect(() => {
        if (!screenOpened || perms.screen === 'granted') return;

        pollRef.current = setInterval(async () => {
            const granted = await window.electron?.ipcRenderer?.invoke('permissions-check-screen') as boolean | undefined;
            if (granted) {
                setPerms(p => ({ ...p, screen: 'granted' }));
                if (pollRef.current) clearInterval(pollRef.current);
            }
        }, POLL_INTERVAL_MS);

        return () => { if (pollRef.current) clearInterval(pollRef.current); };
    }, [screenOpened, perms.screen]);

    const handleRequestMic = async () => {
        if (perms.mic === 'granted' || micLoading) return;
        setMicLoading(true);
        const result = await window.electron?.ipcRenderer?.invoke('permissions-request-mic') as
            { mic: boolean; action: string } | undefined;
        setMicLoading(false);
        if (result?.mic) {
            setPerms(p => ({ ...p, mic: 'granted' }));
        } else {
            setPerms(p => ({ ...p, mic: result?.action === 'open-settings' ? 'denied' : 'pending' }));
        }
    };

    const handleOpenScreenSettings = async () => {
        // Prompt macOS to register the app in the Screen Recording list (first-time only)
        await window.electron?.ipcRenderer?.invoke('permissions-request-screen');
        // Open System Settings directly to the Screen Recording pane
        await window.electron?.ipcRenderer?.invoke('permissions-open-screen-settings');
        setScreenOpened(true);
    };

    const handleComplete = async () => {
        if (!canComplete || completing) return;
        setCompleting(true);
        // Writes flag + relaunches the app
        await window.electron?.ipcRenderer?.invoke('permissions-complete');
    };

    return (
        <div className="permissions-screen">
            <div className="splash-drag-bar"></div>

            <div className="permissions-body">
                <h1 className="permissions-title">Permissions</h1>
                <p className="permissions-subtitle">
                    Sayso needs two permissions to work. We will only capture audio during active calls
                </p>
                <div className="permissions-list">
                    <div className="perm-row">
                        <div className="perm-row-left">
                            <div className={`perm-icon-bg ${perms.mic === 'granted' ? 'perm-icon-bg--granted' : ''}`}>
                                <LuMic size={18} />
                            </div>
                            <div>
								<div className='perm-name-container'>
	                                <p className="perm-name">Microphone</p>
									{ 
										perms.mic === 'granted' &&
										<div className='perm-granted-check'>
											<LuCheck size={14} />
										</div>
									}
								</div>
                                <p className="perm-desc">To capture your side of the conversation during calls</p>
                            </div>
                        </div>
                    </div>

                    <div className="perm-divider" />

                    {/* Screen recording row */}
                    <div className={`perm-row ${perms.mic !== 'granted' ? 'perm-row--disabled' : ''}`}>
                        <div className="perm-row-left">
                            <div className={`perm-icon-bg ${perms.screen === 'granted' ? 'perm-icon-bg--granted' : ''}`}>
                                <LuHeadphones size={18} />
                            </div>
							{ 
								perms.screen === 'granted' &&
								<div className='perm-granted-check'>
									<LuCheck size={12} />
								</div>
							}
                            <div>
                                <p className="perm-name">Screen & System Audio</p>
                                <p className="perm-desc">To capture audio from your leads during conversations</p>
                            </div>
                        </div>
                    </div>
                </div>
				
				{ 
					perms.mic !== 'granted' && perms.screen !== 'granted' && (
						<p className="permissions-hint">
							Allow <strong>Sayso</strong> to use your Microphone.
						</p>
					) 
				}
				{ 
					perms.mic === 'granted' && perms.screen !== 'granted' && (
						<p className="permissions-hint">
							Enable <strong>Sayso</strong> in System Settings → Privacy &amp; Security → Screen Recording, then quit and reopen the app.
						</p>
					) 
				}

                {!canComplete && (
                    <button
                        className="permissions-cta-btn"
                        onClick={perms.mic !== 'granted' ? handleRequestMic : handleOpenScreenSettings}
                        disabled={micLoading}
                    >
                        {micLoading
                            ? <><LuLoader size={14} className="perm-spin" /> Requesting…</>
                            : perms.mic !== 'granted'
                                ? 'Allow Microphone'
                                : screenOpened ? 'Open System Settings' : 'Open System Settings'
                        }
                    </button>
                )}

                {canComplete && (
                    <button
                        className="permissions-complete-btn"
                        onClick={handleComplete}
                        disabled={completing}
                    >
                        {completing
                            ? <><LuLoader size={14} className="perm-spin" /> Restarting…</>
                            : 'Quit and Reopen →'
                        }
                    </button>
                )}
            </div>
        </div>
    );
}
