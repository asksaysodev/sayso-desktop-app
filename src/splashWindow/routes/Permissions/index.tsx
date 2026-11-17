import { useEffect, useRef, useState } from 'react';
import { LuCheck, LuMic, LuLoader, LuHeadphones } from 'react-icons/lu';
import { useNavigate } from 'react-router-dom';
import type { PermissionRequirements, PermissionsCheckResult, SystemAudioPermissionState } from '@/types/permissions';
import './styles.css';

type PermStatus = 'pending' | 'granted' | 'denied';

interface PermState {
    mic: PermStatus;
    screen: PermStatus;
}

const POLL_INTERVAL_MS = 1500;
// Assume the macOS shape until the first permissions-check resolves
const DEFAULT_REQUIREMENTS: PermissionRequirements = { screen: true, relaunchOnComplete: false, systemAudio: 'audio-capture' };

export default function Permissions() {
    const [perms, setPerms] = useState<PermState>({ mic: 'pending', screen: 'pending' });
    const [requirements, setRequirements] = useState<PermissionRequirements>(DEFAULT_REQUIREMENTS);
    const [systemAudioState, setSystemAudioState] = useState<SystemAudioPermissionState | null>(null);
    const [micLoading, setMicLoading] = useState(false);
    const [screenLoading, setScreenLoading] = useState(false);
    const [screenOpened, setScreenOpened] = useState(false);
    const [completing, setCompleting] = useState(false);
    const pollRef = useRef<ReturnType<typeof setInterval> | null>(null);
    const navigate = useNavigate();
    const needsScreen = requirements.screen;
    const relaunches = requirements.relaunchOnComplete;
    const liveAudio = requirements.systemAudio === 'audio-capture';

    // macOS binds screen-recording permission at process launch — a mid-session grant is NEVER
    // reflected to the running process (confirmed: CGPreflight returns false until restart). So we
    // can't detect SCK live. Once mic is granted AND the user has gone to System Settings, offer
    // "Quit and Reopen"; the next launch is the source of truth for whether SCK was actually granted.
    // Where the OS gates no screen capture (Windows), a granted mic is the whole story.
    const canComplete = perms.mic === 'granted'
        && (!needsScreen || perms.screen === 'granted' || (!liveAudio && screenOpened));

    // On mount, check current state so returning users see accurate status
    useEffect(() => {
        const init = async () => {
            const result = await window.electron?.ipcRenderer?.invoke('permissions-check') as PermissionsCheckResult | undefined;
            if (result) {
                setPerms({
                    mic: result.mic ? 'granted' : 'pending',
                    screen: result.screen ? 'granted' : 'pending',
                });
                setSystemAudioState(result.systemAudio ?? null);
                setRequirements(result.requirements);
            }
        };
        init();
    }, []);

    // Poll for screen recording grant once user has opened settings
    useEffect(() => {
        if (!needsScreen || perms.screen === 'granted') return;
        if (liveAudio ? perms.mic !== 'granted' : !screenOpened) return;

        pollRef.current = setInterval(async () => {
            const result = await window.electron?.ipcRenderer?.invoke('permissions-check') as PermissionsCheckResult | undefined;
            if (!result) return;
            setSystemAudioState(result.systemAudio ?? null);
            if (result.screen) {
                setPerms(p => ({ ...p, screen: 'granted' }));
                if (pollRef.current) clearInterval(pollRef.current);
            }
        }, POLL_INTERVAL_MS);

        return () => { if (pollRef.current) clearInterval(pollRef.current); };
    }, [needsScreen, liveAudio, screenOpened, perms.mic, perms.screen]);

    useEffect(() => {
        if (relaunches || perms.mic === 'granted') return;

        const id = setInterval(async () => {
            const result = await window.electron?.ipcRenderer?.invoke('permissions-check') as PermissionsCheckResult | undefined;
            if (result?.mic) setPerms(p => ({ ...p, mic: 'granted' }));
        }, POLL_INTERVAL_MS);

        return () => clearInterval(id);
    }, [relaunches, perms.mic]);

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
        if (screenLoading) return;
        if (liveAudio) {
            if (systemAudioState !== 'denied' && !screenOpened) {
                setScreenLoading(true);
                const state = await window.electron?.ipcRenderer?.invoke('permissions-request-screen') as
                    SystemAudioPermissionState | null | undefined;
                setScreenLoading(false);
                setScreenOpened(true);
                if (state) setSystemAudioState(state);
                if (state === 'granted') setPerms(p => ({ ...p, screen: 'granted' }));
                if (state !== 'denied') return;
            }
            await window.electron?.ipcRenderer?.invoke('permissions-open-screen-settings');
            setScreenOpened(true);
            return;
        }
        if (!screenOpened) {
            // First action: trigger the native macOS dialog (it has its own "Open System Settings" button).
            // macOS only shows it when the permission is not-determined.
            await window.electron?.ipcRenderer?.invoke('permissions-request-screen');
            setScreenOpened(true);
        } else {
            // Already prompted: if the user previously denied, macOS won't re-show the dialog,
            // so open the Screen Recording pane directly as a fallback.
            await window.electron?.ipcRenderer?.invoke('permissions-open-screen-settings');
        }
    };

    const handleComplete = async () => {
        if (!canComplete || completing) return;
        setCompleting(true);
        // Writes the completion flag and, where the OS needs it, relaunches. On a write
        // failure the app does NOT relaunch and returns an error — reset so the user can
        // retry instead of being stuck on the spinner.
        const result = await window.electron?.ipcRenderer?.invoke('permissions-complete') as { error?: string } | undefined;
        if (result?.error) {
            console.error('[Permissions] Failed to complete:', result.error);
            setCompleting(false);
            return;
        }
        if (!relaunches) navigate('/', { replace: true });
    };

    return (
        <div className="permissions-screen">
            <div className="splash-drag-bar"></div>

            <div className="permissions-body">
                <h1 className="permissions-title">Permissions</h1>
                <p className="permissions-subtitle">
                    {needsScreen
                        ? 'Sayso needs two permissions to work. We will only capture audio during active calls'
                        : 'Sayso needs microphone access to work. We will only capture audio during active calls'}
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

                    {needsScreen && (<>
                    <div className="perm-divider" />

                    {/* Screen recording row */}
                    <div className={`perm-row ${perms.mic !== 'granted' ? 'perm-row--disabled' : ''}`}>
                        <div className="perm-row-left">
                            <div className={`perm-icon-bg ${perms.screen === 'granted' ? 'perm-icon-bg--granted' : ''}`}>
                                <LuHeadphones size={18} />
                            </div>
                            <div>
								<div className='perm-name-container'>
									<p className="perm-name">{liveAudio ? 'System Audio' : 'Screen & System Audio'}</p>
									{ 
										perms.screen === 'granted' &&
										<div className='perm-granted-check'>
											<LuCheck size={12} />
										</div>
									}
								</div>
								<p className="perm-desc">To capture audio from your leads during conversations</p>
                            </div>
                        </div>
                    </div>
                    </>)}
                </div>
				
				{ 
					needsScreen && perms.mic !== 'granted' && perms.screen !== 'granted' && (
						<p className="permissions-hint">
							Allow <strong>Sayso</strong> to use your Microphone.
						</p>
					) 
				}
				{ 
					needsScreen && !liveAudio && perms.mic === 'granted' && perms.screen !== 'granted' && (
						<p className="permissions-hint">
							Enable <strong>Sayso</strong> in System Settings → Privacy &amp; Security → Screen Recording, then quit and reopen the app.
						</p>
					) 
				}
				{ 
					liveAudio && perms.mic === 'granted' && perms.screen !== 'granted' && systemAudioState !== 'denied' && (
						<p className="permissions-hint">
							Allow <strong>Sayso</strong> to record system audio when macOS asks. No restart needed.
						</p>
					)
				}
				{
					liveAudio && perms.mic === 'granted' && perms.screen !== 'granted' && systemAudioState === 'denied' && (
						<p className="permissions-hint">
							System audio is turned off for <strong>Sayso</strong>. Enable it in System Settings → Privacy &amp; Security → Screen &amp; System Audio Recording → System Audio Recording Only. If macOS then offers to quit and reopen, choose <strong>Later</strong> — no restart is needed.
						</p>
					)
				}
				{
					!needsScreen && perms.mic !== 'granted' && (
						<p className="permissions-hint">
							Turn on <strong>Microphone access</strong> and <strong>Let desktop apps access your microphone</strong> in Settings → Privacy &amp; security → Microphone.
						</p>
					) 
				}

                {!canComplete && (
                    <button
                        className="permissions-cta-btn"
                        onClick={perms.mic !== 'granted' ? handleRequestMic : handleOpenScreenSettings}
                        disabled={micLoading || screenLoading}
                    >
                        {micLoading || screenLoading
                            ? <><LuLoader size={14} className="perm-spin" /> Requesting…</>
                            : perms.mic !== 'granted'
                                ? (needsScreen ? 'Allow Microphone' : 'Open Microphone Settings')
                                : liveAudio && systemAudioState !== 'denied' && !screenOpened
                                    ? 'Allow System Audio'
                                    : 'Open System Settings'
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
                            ? <><LuLoader size={14} className="perm-spin" /> {relaunches ? 'Restarting…' : 'Continuing…'}</>
                            : relaunches ? 'Quit and Reopen →' : 'Continue →'
                        }
                    </button>
                )}
            </div>
        </div>
    );
}
