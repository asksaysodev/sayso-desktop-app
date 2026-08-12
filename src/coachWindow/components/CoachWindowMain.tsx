import { useEffect, useRef, useState, useCallback, useMemo } from 'react';
import { useSessionExpiry } from '@/hooks/useSessionExpiry';
import { useNetworkState } from '@/hooks/useNetworkState';
import { MdDragIndicator } from 'react-icons/md';
import { MdErrorOutline } from 'react-icons/md';
import { LuX } from 'react-icons/lu';
import { useCoachWindowStore } from '../../store/coachWindowStore';
import CoachButtons from './CoachButtons';
import SelectLeadTypeDropdown from './SelectLeadTypeDropdown';
import InsightsVerticalLayout from './InsightsVerticalLayout';
import SessionStoppedDialog from './SessionStoppedDialog';
import RightSideButtons from './RightSideButtons';
import useFontSize from '../hooks/useFontSize';
import usePulseMarketProperty from '../hooks/usePulseMarketProperty';
import ZipCodeDropdown from './ZipCodeDropdown';
import { usePlaybookPrefetch } from '@/playbookWindow/hooks/usePlaybookPrefetch';
import Pulse from './Pulse';
import { reportCoachError } from '@/utils/errorReporting';
import { CUE_CONNECTIVITY_MESSAGE } from '../helpers/cueErrorMessage';

const WINDOW_WIDTH_SIZES = {
    s: { BASE: 380, MAX_WIDTH: 900 },
    m: { BASE: 400, MAX_WIDTH: 900 },
    l: { BASE: 420, MAX_WIDTH: 900 },
}

const WINDOW_HEIGHT_SIZES = {
    BASE: 54,
    DROPDOWN_OPEN: 220,
    ERROR: 110,
}


export default function CoachWindowMain() {
    //REFS
    const containerRef = useRef<HTMLDivElement | null>(null);
    const mainContainerRef = useRef<HTMLDivElement | null>(null);
    const insightsLayoutRef = useRef<HTMLDivElement | null>(null);
    const zipCodeDropdownRef = useRef<HTMLDivElement | null>(null);
    const errorContainerRef = useRef<HTMLDivElement | null>(null);
    const sessionStoppedDialogRef = useRef<HTMLDivElement | null>(null);
    // Mirror of insightsListMaxHeight state as a ref so getHeightByCurrentState()
    // can read the latest computed value synchronously without waiting for a React re-render.
    const insightsListMaxHeightRef = useRef<number | undefined>(undefined);
    // Absolute bottom of the work area (screen coords) from the main process.
    // Populated on mount via IPC; falls back to screen.availHeight (conservative by ~menuBarHeight).
    const workAreaBottomRef = useRef<number | null>(null);
    //STATE
    const [isDropdownOpen, setIsDropdownOpen] = useState(false);
    const [showSessionAutoStopped, setShowSessionAutoStopped] = useState(false);
    const [lpmamaTooltipHeight, setLpmamaTooltipHeight] = useState(0);
    //CONTEXT / HOOKS
    const sessionData = useCoachWindowStore(state => state.sessionData);
    const isCoachActive = useCoachWindowStore(state => state.isCoachActive);
    const coachFeature = useCoachWindowStore(state => state.coachFeature);
    const leadType = useCoachWindowStore(state => state.cue.leadType);
    const currentInsight = useCoachWindowStore(state => state.cue.currentInsight);
    const insightsQueue = useCoachWindowStore(state => state.cue.insightsQueue);
    const isInsightsLayoutOpen = useCoachWindowStore(state => state.cue.isInsightsLayoutOpen);
    const setIsInsightsLayoutOpen = useCoachWindowStore(state => state.cue_setIsInsightsLayoutOpen);
    const coachWindowError = useCoachWindowStore(state => state.error);
    const clearError = useCoachWindowStore(state => state.clearError);
    const hasReceivedFirstInsight = useCoachWindowStore(state => state.cue.hasReceivedFirstInsight);
    const setHasReceivedFirstInsight = useCoachWindowStore(state => state.cue_setHasReceivedFirstInsight);
    const incrementUnseenInsightsCount = useCoachWindowStore(state => state.cue_incrementUnseenInsightsCount);
    const handleStopCue = useCoachWindowStore(state => state.cue_handleStopCue);
    const onPressResetSession = useCoachWindowStore(state => state.cue_onPressResetSession);
    const isSmartCaptureEnabled = useCoachWindowStore(state => state.cue.enabledFeatures.includes('smart_capture'));
    const isPulseEnabled = useCoachWindowStore(state => state.cue.enabledFeatures.includes('pulse'));
    const currentfs = useFontSize();
    const { isReconnecting } = useNetworkState();
    usePlaybookPrefetch();
    const [zipCodeValue, setZipCodeValue] = useState<string>("")
    const [isZipDropdownOpen, setIsZipDropdownOpen] = useState<boolean>(false);
    const [insightsListMaxHeight, setInsightsListMaxHeight] = useState<number | undefined>(undefined);

    const isZipCodeValid = useMemo(()=> {
        return /^\d{5}$/.test(zipCodeValue);
    },[zipCodeValue])

    const {
        allResults,
        pulseError,
        isPending: isPulsePending,
        retry: retryPulse,
    } = usePulseMarketProperty(zipCodeValue, sessionData?.sessionId ?? '');

    useEffect(() => {
        if (isZipCodeValid) setIsZipDropdownOpen(true);
    }, [isZipCodeValid]);

    // Fetch the absolute bottom of the work area from the main process so we can
    // accurately constrain the insights list without relying on availTop (Firefox-only).
    useEffect(() => {
        window.electronAPI?.getCoachWorkAreaBottom()?.then(v => { workAreaBottomRef.current = v; });
    }, []);

    const handleZipClose = useCallback(() => setIsZipDropdownOpen(false), []);

    // Compute the max height for the scrollable insights list by measuring the
    // list's actual viewport position after DOM layout. This avoids stale-ref issues
    // that occur when reading offsetHeight during the render phase.
    const updateInsightsMaxListHeight = useCallback(() => {
        if (!isInsightsLayoutOpen || !insightsLayoutRef.current) {
            insightsListMaxHeightRef.current = undefined;
            setInsightsListMaxHeight(undefined);
            return;
        }
        const listEl = insightsLayoutRef.current.querySelector<HTMLElement>('ul.insights-vertical-list');
        if (!listEl) return;
        const listTop = listEl.getBoundingClientRect().top;
        if (listTop <= 0) return; // not yet in viewport
        // Reserve: lpmama row (36px) + container bottom padding (9px) + gap after container (6px)
        const LPMAMA_RESERVE = isSmartCaptureEnabled ? 51 : 0;
        const BOTTOM_MARGIN = 20;
        // Use IPC-provided workAreaBottom (accurate). Falls back to screen.availHeight,
        // which is conservative by ~menuBarHeight but safe.
        const screenBottom = workAreaBottomRef.current ?? window.screen.availHeight;
        const available = screenBottom - window.screenY - listTop - LPMAMA_RESERVE - BOTTOM_MARGIN;
        const newMax = Math.max(150, Math.round(available));
        // Update the ref synchronously so getHeightByCurrentState() can use it immediately
        // in the same call stack, before React re-renders with the new state.
        insightsListMaxHeightRef.current = newMax;
        setInsightsListMaxHeight(prev => prev !== newMax ? newMax : prev);
    }, [isInsightsLayoutOpen, isSmartCaptureEnabled]);
    
    // By the time this fires the credentials are already gone, so POST /cue/session/stop
    // could only 401 — persisting the session is main's job now, before it tears auth
    // down (stopAndPersistCueSession in electron/main.ts). Here we only release
    // local resources. Reads the store directly so a stale closure can't skip the
    // teardown. SAYSO-335.
    const handleSessionExpired = useCallback(() => {
        useCoachWindowStore.setState({ error: 'Your session has expired. Please re-login from the main window.' });
        useCoachWindowStore.getState().cue_handleLocalTeardown().catch(reportCoachError);
    }, []);
    useSessionExpiry(handleSessionExpired);

    // On transition from offline → online, clear any stale error left over from
    // the outage (e.g. an "expired token" or failed API call that fired while the
    // network was down). Only fires on the actual transition, not on mount.
    const wasReconnecting = useRef(false);
    useEffect(() => {
        if (wasReconnecting.current && !isReconnecting) {
            useCoachWindowStore.getState().clearError();
        }
        wasReconnecting.current = isReconnecting;
    }, [isReconnecting]);

    const runCueAction = useCallback((action: () => Promise<unknown>) => {
        action().catch(reportCoachError);
    }, []);

    const handleRequestStop = useCallback(() => {
        runCueAction(handleStopCue);
    }, [runCueAction, handleStopCue]);

    const handleRequestReset = useCallback(() => {
        runCueAction(onPressResetSession);
    }, [runCueAction, onPressResetSession]);


    function getTotalHeightWithRef(ref: any) {
        if (!ref || !ref?.current) return 0;

        const actualHeight = ref.current?.offsetHeight ?? 0;
        const totalHeight = WINDOW_HEIGHT_SIZES.BASE + actualHeight + 6;
        return totalHeight; 
    }
    
    function getWidthByCurrentState() {
        const fsWidthOptions = WINDOW_WIDTH_SIZES[currentfs];
        const natural = mainContainerRef.current?.offsetWidth ?? fsWidthOptions.BASE;
        return Math.min(fsWidthOptions.MAX_WIDTH, Math.max(fsWidthOptions.BASE, natural));
    }

    function getHeightByCurrentState() {
        const showError = coachWindowError && coachFeature === 'cue' && leadType;
        if (showError) {
            return getTotalHeightWithRef(errorContainerRef);
        }

        if (isDropdownOpen) {
            return WINDOW_HEIGHT_SIZES.DROPDOWN_OPEN;
        }

        if (showSessionAutoStopped) {
            return getTotalHeightWithRef(sessionStoppedDialogRef) || WINDOW_HEIGHT_SIZES.BASE;
        }

        let height = WINDOW_HEIGHT_SIZES.BASE;

        const showZipDropdown = isZipDropdownOpen && isZipCodeValid && isCoachActive && isPulseEnabled;
        if (showZipDropdown && zipCodeDropdownRef.current) {
            height += zipCodeDropdownRef.current.offsetHeight + 6;
        }

        if (isInsightsLayoutOpen) {
            if (insightsLayoutRef.current) {
                const listEl = insightsLayoutRef.current.querySelector<HTMLElement>('ul.insights-vertical-list');
                if (listEl) {
                    // Compute height mathematically using scrollHeight (natural size ignoring
                    // max-height) capped by the ref, which is set synchronously by
                    // updateInsightsMaxListHeight before this function is called.
                    const LPMAMA_HEIGHT = isSmartCaptureEnabled ? 36 : 0;
                    const naturalH = listEl.scrollHeight;
                    const maxH = insightsListMaxHeightRef.current;
                    const listH = maxH !== undefined ? Math.min(naturalH, maxH) : naturalH;
                    height += (18 + listH + LPMAMA_HEIGHT) + 6 + lpmamaTooltipHeight;
                } else {
                    height += insightsLayoutRef.current.offsetHeight + 6 + lpmamaTooltipHeight;
                }
            } else {
                const queueLength = insightsQueue.length;
                let estimatedInsightsHeight = 280;
                if (queueLength === 1) estimatedInsightsHeight = 140;
                else if (queueLength === 2) estimatedInsightsHeight = 210;
                else if (queueLength === 3) estimatedInsightsHeight = 280;
                else if (queueLength >= 4) estimatedInsightsHeight = 350;
                height += estimatedInsightsHeight - WINDOW_HEIGHT_SIZES.BASE;
            }
        }

        return height;
    }

    useEffect(() => {
        let isResizing = false;

        const updateWindowSize = () => {
            if (containerRef.current && window.electronAPI && !isResizing) {
                isResizing = true;
                const windowWidth = getWidthByCurrentState();
                const windowHeight = getHeightByCurrentState();
                window.electronAPI.resizeWindow(windowWidth, windowHeight);

                setTimeout(() => {
                    isResizing = false;
                }, 100);
            }
        };

        // When currentInsight becomes null, add a small delay to ensure DOM has updated
        // When it appears, update immediately
        const delay = currentInsight === null ? 50 : 0;

        let rafId: number | null = null;
        const timeoutId = setTimeout(() => {
            rafId = requestAnimationFrame(() => {
                // First: compute the constraint from DOM (updates ref synchronously).
                updateInsightsMaxListHeight();
                requestAnimationFrame(() => {
                    // Second: resize the window using the now-accurate ref value.
                    updateWindowSize();
                });
            });
        }, delay);

        // ResizeObserver: call both in the same RAF so the ref is already set when
        // updateWindowSize reads it — no second render cycle needed for correct sizing.
        const resizeObserver = new ResizeObserver(() => {
            requestAnimationFrame(() => {
                updateInsightsMaxListHeight(); // sets ref synchronously
                updateWindowSize();            // reads ref → correct height immediately
            });
        });
        if (containerRef.current) {
            resizeObserver.observe(containerRef.current);
        }
        if (mainContainerRef.current) {
            resizeObserver.observe(mainContainerRef.current);
        }
        if (zipCodeDropdownRef.current) {
            resizeObserver.observe(zipCodeDropdownRef.current);
        }
        if (insightsLayoutRef.current) {
            resizeObserver.observe(insightsLayoutRef.current);
        }

        return () => {
            clearTimeout(timeoutId);
            if (rafId) cancelAnimationFrame(rafId);
            resizeObserver.disconnect();
        };
    }, [isDropdownOpen, currentInsight, leadType, insightsQueue, isCoachActive, coachFeature, isInsightsLayoutOpen, coachWindowError, showSessionAutoStopped, currentfs, lpmamaTooltipHeight, isZipCodeValid, isZipDropdownOpen, isPulseEnabled, allResults, pulseError, isPulsePending, updateInsightsMaxListHeight]);

    /**
     * Handling auto opening of insights layout and unseen insights count for the notification dot
     */
    const handleLayoutVisibility = () => {
        if (!hasReceivedFirstInsight) {
            setHasReceivedFirstInsight(true);
            if (!isInsightsLayoutOpen) {
                setIsInsightsLayoutOpen(true);
            }
        } else {
            if (!isInsightsLayoutOpen) {
                incrementUnseenInsightsCount();
            }
        }
    }

	useEffect(() => {
		// Only set up listener when Cue is active
		if (!isCoachActive || coachFeature !== 'cue') return;

		if (!window.electron?.cue?.onInsight) {
			console.warn('⚠️ [CoachWindowMain] Electron cue.onInsight not available');
			return;
		}

		const unsubscribe = window.electron.cue.onInsight((insightData) => {
			const addInsight = useCoachWindowStore.getState().cue_addInsight;

			addInsight({
				message: insightData.message,
				priority: insightData.priority,
				appointmentBooked: insightData.appointmentBooked || false,
                id: insightData.id,
			});

            handleLayoutVisibility();
		});

		return () => {
			unsubscribe();
		};
    }, [isCoachActive, coachFeature, isInsightsLayoutOpen, hasReceivedFirstInsight]);

    useEffect(() => {
        if (!isCoachActive || coachFeature !== 'cue') return;
        if (!isSmartCaptureEnabled) return;
        if (!window.electron?.cue?.onSmartCapture) return;

        const unsubscribe = window.electron.cue.onSmartCapture((data) => {
            const updateSmartCapture = useCoachWindowStore.getState().cue_updateSmartCapture;
            updateSmartCapture(data);
            handleLayoutVisibility();
        });

        return () => { unsubscribe(); };
    }, [isCoachActive, coachFeature, isSmartCaptureEnabled, isInsightsLayoutOpen, hasReceivedFirstInsight]);

    useEffect(() => {
        if (!isCoachActive || coachFeature !== 'cue') return;
        if (!window.electron?.cue?.onLowUserAudio) return;

        const unsubscribe = window.electron.cue.onLowUserAudio(() => {
            useCoachWindowStore.setState({
                error:
                    "Sayso isn't receiving microphone audio yet. Check your input device, or wait a few seconds after stopping before starting again.",
            });
        });

        return () => {
            unsubscribe();
        };
    }, [isCoachActive, coachFeature]);

    /**
     * Consumes `cue-error` (audioManager.ts:380). Until now nothing did, so a streaming
     * socket that died mid-session was silent (SAYSO-346).
     *
     * Scoped to a live session on purpose. A socket failure *at start* rejects
     * audioStreamer.start() as well, so cue_handleStartCue already surfaces it — without
     * the isCoachActive gate the same failure would arrive twice, and the second one
     * would land after the store had reset.
     *
     * The every-attempt firing is why `notified` exists: websocketClient re-emits on each
     * reconnect, and the closure resets per session because this effect re-subscribes
     * when isCoachActive flips.
     */
    useEffect(() => {
        if (!isCoachActive || coachFeature !== 'cue') return;
        if (!window.electron?.cue?.onError) return;

        let notified = false;
        const unsubscribe = window.electron.cue.onError(() => {
            if (notified) return;
            notified = true;
            // The only producer is a WebSocket failure, so the reason is always
            // connectivity — and the raw payload is an ENOTFOUND-class string that
            // means nothing to the user.
            useCoachWindowStore.setState({ error: CUE_CONNECTIVITY_MESSAGE });
        });

        return () => {
            unsubscribe();
        };
    }, [isCoachActive, coachFeature]);

    useEffect(() => {
        if (!window.electron?.cue?.onAutoStop) {
            console.warn('⚠️ [CoachWindowMain] Electron cue.onAutoStop not available');
            return;
        }

        const unsubscribe = window.electron.cue.onAutoStop(async () => {
            setIsInsightsLayoutOpen(false);
            try {
                await handleStopCue();
            } catch (error) {
                reportCoachError(error);
            } finally {
                setShowSessionAutoStopped(true);
            }
        });

        return () => {
            unsubscribe();
        };
    }, [])

    useEffect(() => {
        if (leadType && (isDropdownOpen || isCoachActive)) {
            setShowSessionAutoStopped(false);
        }
    }, [isDropdownOpen, isCoachActive, leadType]);

    const DropdownComponent = {
        cue: <SelectLeadTypeDropdown isDropdownOpen={isDropdownOpen} setIsDropdownOpen={setIsDropdownOpen} />,
    };
    
    useEffect(() => {
        if (!sessionData) {
            setZipCodeValue('');
            setIsZipDropdownOpen(false);
        }
    }, [sessionData])
    
    return (
        <div className={`coach-window${isDropdownOpen && coachFeature === 'cue' ? ' dropdown-open' : ''}`} ref={containerRef}>
            <div className={`main-container coach-box-bubble`} ref={mainContainerRef}>
                <div className='main-toolbar'>
                    <div className="coach-window-drag-container">
                        <div className='coach-window-drag-indicator'>
                            <MdDragIndicator/>
                        </div>
                        <div className="coach-window-divider"></div>
                    </div>
                    
                    <div className='inner-content-centered'>
                        {
                            DropdownComponent[coachFeature] && (
                                DropdownComponent[coachFeature]
                            )
                        }
                        
                        <Pulse
                            isCoachActive={isCoachActive}
                            isPulseEnabled={isPulseEnabled}
                            isZipCodeValid={isZipCodeValid}
                            setIsZipDropdownOpen={setIsZipDropdownOpen}
                            isZipDropdownOpen={isZipDropdownOpen}
                            setZipCodeValue={setZipCodeValue}
                            zipCodeValue={zipCodeValue}
                        />

                        {isCoachActive && !isPulseEnabled && (
                            <div className="pulse-holder" />
                        )}

                        {
                            leadType && (
                                <CoachButtons
                                    setIsDropdownOpen={setIsDropdownOpen}
                                    isDropdownOpen={isDropdownOpen}
                                    onRequestStop={coachFeature === 'cue' ? handleRequestStop : undefined}
                                    onRequestReset={coachFeature === 'cue' ? handleRequestReset : undefined}
                                />
                            )
                        }
                    </div>

                    <RightSideButtons />
                </div>
            </div>

            {isReconnecting && coachFeature === 'cue' && leadType && (
                <div className='cue-error-container coach-box-bubble' ref={errorContainerRef}>
                    <MdErrorOutline className='cue-error-icon' />
                    <span className='cue-error-text'>No internet connection — we&apos;ll reconnect automatically.</span>
                </div>
            )}

            {!isReconnecting && coachWindowError && coachFeature === 'cue' && leadType && (
                <div className='cue-error-container coach-box-bubble' ref={errorContainerRef}>
                    <MdErrorOutline className='cue-error-icon' />
                    <span className='cue-error-text'>{coachWindowError}</span>
                    <button
                        className='cue-error-close-button'
                        onClick={() => clearError()}
                        aria-label="Close error message"
                    >
                        <LuX />
                    </button>
                </div>
            )}

            {isZipDropdownOpen && isZipCodeValid && isCoachActive && isPulseEnabled && (
                <ZipCodeDropdown
                    ref={zipCodeDropdownRef}
                    onClose={handleZipClose}
                    zipCodeValue={zipCodeValue}
                    allResults={allResults}
                    pulseError={pulseError}
                    isPending={isPulsePending}
                    onRetry={retryPulse}
                />
            )}
            
            {isInsightsLayoutOpen && isCoachActive && (
                <InsightsVerticalLayout
                    ref={insightsLayoutRef}
                    onLpmamaTooltipHeightChange={setLpmamaTooltipHeight}
                    maxListHeight={insightsListMaxHeight}
                />
            )}

            {showSessionAutoStopped && leadType && (
                <SessionStoppedDialog
                    setShowSessionAutoStopped={setShowSessionAutoStopped}
                    ref={sessionStoppedDialogRef}
                />
            )}
        </div>
    );
}
