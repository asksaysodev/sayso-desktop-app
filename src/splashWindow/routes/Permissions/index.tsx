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
// Success sequence (tap backend, where grants are detected live): each row's icon turns green as it's
// granted; once everything is granted the rest of the screen leaves and the centered check draws in.
const GRANTED_HOLD_MS = 900;
const LEAVE_MS = 450;
type SuccessPhase = 'idle' | 'granted' | 'leaving' | 'done';
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
    // Existing user re-granting after the move to Core Audio taps (Screen Recording already granted,
    // System Audio still to grant). Gets its own explanation; first-run onboarding is unchanged.
    const [upgrade, setUpgrade] = useState(false);
    const [phase, setPhase] = useState<SuccessPhase>('idle');
    // Rows granted while this screen is open get the pop + check icon. Grants that were already in
    // place on mount keep the static granted style.
    const [popped, setPopped] = useState<{ mic: boolean; screen: boolean }>({ mic: false, screen: false });
    const initialPermsRef = useRef<PermState | null>(null);
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
    // An upgrading user who declines keeps working on ScreenCaptureKit (spec S-3), so a denial is an
    // answer, not a dead end. New users have no such fallback and still need the grant.
    const upgradeDeclined = upgrade && systemAudioState === 'denied';
    // From the state on mount: an upgrading user who also has to grant the mic keeps the row (and its
    // grant animation) instead of it vanishing the moment the mic is granted.
    const [micGrantedOnMount, setMicGrantedOnMount] = useState(false);
    const hideMic = upgrade && micGrantedOnMount;
    const celebrating = phase !== 'idle';
    // True from the render where the last grant lands, one frame before the phase effect runs, so the
    // Continue button never flashes in ahead of the animation.
    const autoSuccess = liveAudio && !relaunches && perms.mic === 'granted' && perms.screen === 'granted';

    useEffect(() => {
        const initial = initialPermsRef.current;
        if (!initial) return;
        if (perms.mic === 'granted' && initial.mic !== 'granted') setPopped(p => (p.mic ? p : { ...p, mic: true }));
        if (perms.screen === 'granted' && initial.screen !== 'granted') setPopped(p => (p.screen ? p : { ...p, screen: true }));
    }, [perms.mic, perms.screen]);

    // Everything granted on the tap backend: record completion right away (no relaunch needed) and
    // play the success sequence. Upgrading users then close the window themselves; new users are
    // taken on to the app. The ScreenCaptureKit backend can't see a grant until relaunch, so it keeps
    // the "Quit and Reopen" button.
    useEffect(() => {
        if (!liveAudio || relaunches || phase !== 'idle') return;
        if (perms.mic !== 'granted' || perms.screen !== 'granted') return;
        setPhase('granted');
        window.electron?.ipcRenderer?.invoke('permissions-complete').then((result) => {
            const error = (result as { error?: string } | undefined)?.error;
            if (error) console.error('[Permissions] Failed to complete:', error);
        });
    }, [liveAudio, relaunches, perms.mic, perms.screen, phase]);

    useEffect(() => {
        if (phase === 'granted') {
            const id = setTimeout(() => setPhase('leaving'), GRANTED_HOLD_MS);
            return () => clearTimeout(id);
        }
        if (phase === 'leaving') {
            const id = setTimeout(() => setPhase('done'), LEAVE_MS);
            return () => clearTimeout(id);
        }
    }, [phase]);

    // New user's "Launch Coach": main finishes the splash as after any completed permissions step,
    // then opens the coach, or the onboarding tour instead when it's due (the tour leads to the coach).
    const handleLaunchCoach = () => {
        window.electron?.ipcRenderer?.send('splash-login-success', { openCoach: true });
    };

    const canComplete = perms.mic === 'granted'
        && (!needsScreen || perms.screen === 'granted' || (!liveAudio && screenOpened) || upgradeDeclined);

    // On mount, check current state so returning users see accurate status
    useEffect(() => {
        const init = async () => {
            const result = await window.electron?.ipcRenderer?.invoke('permissions-check') as PermissionsCheckResult | undefined;
            if (result) {
                const initial: PermState = {
                    mic: result.mic ? 'granted' : 'pending',
                    screen: result.screen ? 'granted' : 'pending',
                };
                initialPermsRef.current = initial;
                setMicGrantedOnMount(initial.mic === 'granted');
                setPerms(initial);
                setSystemAudioState(result.systemAudio ?? null);
                setRequirements(result.requirements);
                setUpgrade(!!result.upgrade);
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
                // Upgrading users who decline get "Continue for now", not a trip to System Settings.
                if (state !== 'denied' || upgrade) return;
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

            {phase === 'done' ? (
                <div className="perm-success">
                    <div className="perm-success-center">
                        <svg className="perm-success-check" viewBox="0 0 72 72" aria-hidden="true">
                            <circle className="perm-success-ring" cx="36" cy="36" r="33" />
                            <path className="perm-success-tick" d="M22 37.5 L31.5 47 L50 27" />
                        </svg>
                        <div className="perm-success-copy">
                            <p className="perm-success-title">{upgrade ? 'Permissions have been updated' : 'Permissions granted'}</p>
                            <p className="perm-success-sub">{upgrade ? 'You can close this window now.' : 'You can start using Sayso now.'}</p>
                        </div>
                    </div>
                    {!upgrade && (
                        <button className="permissions-cta-btn perm-success-cta" onClick={handleLaunchCoach}>
                            Launch Coach
                        </button>
                    )}
                </div>
            ) : (
            <div className={`permissions-body ${upgrade ? 'permissions-body--upgrade' : ''} ${phase === 'leaving' ? 'permissions-body--leaving' : ''}`}>
                <h1 className={`permissions-title ${upgrade ? 'permissions-title--solo' : ''}`}>{upgrade ? 'We’ve improved audio capture for more reliable prompting' : 'Grant permissions'}</h1>
                {!upgrade && <p className="permissions-subtitle">
                    {needsScreen
                            ? 'Sayso needs two permissions to work. We will only capture audio during active calls'
                            : 'Sayso needs microphone access to work. We will only capture audio during active calls'}
                </p>}
                <div className="permissions-list">
                    {/* Upgrading users already granted the mic, so the screen only asks for System Audio. */}
                    {!hideMic && (<>
                    <div className="perm-row">
                        <div className="perm-row-left">
                            <div className={`perm-icon-bg ${perms.mic === 'granted' ? 'perm-icon-bg--granted' : ''} ${popped.mic ? 'perm-icon-bg--pop' : ''}`}>
                                {popped.mic ? <LuCheck size={18} className="perm-icon-check" /> : <LuMic size={18} />}
                            </div>
                            <div>
								<div className='perm-name-container'>
	                                <p className="perm-name">Microphone</p>
									{ 
										perms.mic === 'granted' && !popped.mic &&
										<div className='perm-granted-check'>
											<LuCheck size={14} />
										</div>
									}
								</div>
                                <p className="perm-desc">To capture your side of the conversation during calls</p>
                            </div>
                        </div>
                    </div>

                    </>)}

                    {needsScreen && (<>
                    {!hideMic && <div className="perm-divider" />}

                    {/* Screen recording row */}
                    <div className={`perm-row ${perms.mic !== 'granted' ? 'perm-row--disabled' : ''}`}>
                        <div className="perm-row-left">
                            <div className={`perm-icon-bg ${perms.screen === 'granted' ? 'perm-icon-bg--granted' : ''} ${popped.screen ? 'perm-icon-bg--pop' : ''}`}>
                                {popped.screen ? <LuCheck size={18} className="perm-icon-check" /> : <LuHeadphones size={18} />}
                            </div>
                            <div>
								<div className='perm-name-container'>
									<p className="perm-name">{liveAudio ? 'System Audio' : 'Screen & System Audio'}</p>
									{
										upgrade && perms.screen !== 'granted' &&
										<span className='perm-new-tag'>New</span>
									}
									{ 
										perms.screen === 'granted' && !popped.screen &&
										<div className='perm-granted-check'>
											<LuCheck size={12} />
										</div>
									}
								</div>
								<p className="perm-desc">{upgrade ? 'To capture your prospect’s audio during conversations' : 'To capture audio from your leads during conversations'}</p>
                            </div>
                        </div>
                    </div>
                    </>)}
                </div>
				
				{ 
					needsScreen && !liveAudio && perms.mic !== 'granted' && perms.screen !== 'granted' && (
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
					upgradeDeclined && perms.mic === 'granted' && perms.screen !== 'granted' && (
						<p className="permissions-hint">
							You can turn this on anytime in System Settings. You can close this window now.
						</p>
					)
				}
				{
					!upgrade && liveAudio && perms.mic === 'granted' && perms.screen !== 'granted' && systemAudioState === 'denied' && (
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

                {/* On the upgrade screen the CTA stays mounted once it's no longer needed (granted or
                    declined), faded out: it anchors the bottom so the centered title + row don't jump. */}
                {(!canComplete || upgrade) && (
                    <button
                        className={`permissions-cta-btn ${upgrade ? 'permissions-cta-btn--bottom' : ''} ${canComplete ? 'permissions-cta-btn--gone' : ''}`}
                        onClick={perms.mic !== 'granted' ? handleRequestMic : handleOpenScreenSettings}
                        disabled={micLoading || screenLoading || canComplete}
                        aria-hidden={canComplete || undefined}
                        tabIndex={canComplete ? -1 : undefined}
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

                {canComplete && !celebrating && !autoSuccess && !upgradeDeclined && (
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
            )}

        </div>
    );
}
