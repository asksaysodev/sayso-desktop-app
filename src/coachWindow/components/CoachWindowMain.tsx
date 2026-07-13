import { useEffect, useRef, useState, useCallback, useMemo } from 'react';
import { useSessionExpiry } from '@/hooks/useSessionExpiry';
import { useNetworkState } from '@/hooks/useNetworkState';
import { MdDragIndicator } from 'react-icons/md';
import { MdErrorOutline } from 'react-icons/md';
import { LuX } from 'react-icons/lu';
import * as Sentry from "@sentry/electron/renderer";
import { useCoachWindowStore } from '../../store/coachWindowStore';
import CoachButtons from './CoachButtons';
import SelectLeadTypeDropdown from './SelectLeadTypeDropdown';
import InsightsVerticalLayout from './InsightsVerticalLayout';
import SessionStoppedDialog from './SessionStoppedDialog';
import SmartCaptureWarningDialog, { SmartCaptureWarningMode } from './SmartCaptureWarningDialog';
import RightSideButtons from './RightSideButtons';
import useFontSize from '../hooks/useFontSize';
import usePulseMarketProperty from '../hooks/usePulseMarketProperty';
import ZipCodeDropdown from './ZipCodeDropdown';
import { usePlaybookPrefetch } from '@/playbookWindow/hooks/usePlaybookPrefetch';
import Pulse from './Pulse';
import { copyLpmamaContent, hasCapturedLpmamaData } from '../helpers/copyLpmamaContent';
import { isCuePermissionsDeniedError } from '../services/cueService';

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
    const smartCaptureWarningDialogRef = useRef<HTMLDivElement | null>(null);
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
    const [pendingSmartCaptureAction, setPendingSmartCaptureAction] = useState<SmartCaptureWarningMode | null>(null);
    const [isSmartCaptureProcessing, setIsSmartCaptureProcessing] = useState(false);
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
    
    const handleSessionExpired = useCallback(() => {
        useCoachWindowStore.setState({ error: 'Your session has expired. Please re-login from the main window.' });
        if (isCoachActive) handleStopCue().catch((err) => Sentry.captureException(err));
    }, [isCoachActive, handleStopCue]);
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

    const executeSmartCaptureAction = useCallback((mode: SmartCaptureWarningMode) => {
        const action = mode === 'reset' ? onPressResetSession : handleStopCue;
        action().catch((err) => {
            if (!isCuePermissionsDeniedError(err)) {
                Sentry.captureException(err);
            }
        });
    }, [handleStopCue, onPressResetSession]);

    const handleRequestStop = useCallback(() => {
        const lpmama = useCoachWindowStore.getState().cue.lpmama;
        if (isSmartCaptureEnabled && hasCapturedLpmamaData(lpmama)) {
            setPendingSmartCaptureAction('stop');
            return;
        }
        executeSmartCaptureAction('stop');
    }, [isSmartCaptureEnabled, executeSmartCaptureAction]);

    const handleRequestReset = useCallback(() => {
        const lpmama = useCoachWindowStore.getState().cue.lpmama;
        if (isSmartCaptureEnabled && hasCapturedLpmamaData(lpmama)) {
            setPendingSmartCaptureAction('reset');
            return;
        }
        executeSmartCaptureAction('reset');
    }, [isSmartCaptureEnabled, executeSmartCaptureAction]);

    const handleSmartCaptureCopyAndProceed = useCallback(async () => {
        const mode = pendingSmartCaptureAction;
        if (!mode) return;
        setIsSmartCaptureProcessing(true);
        const lpmama = useCoachWindowStore.getState().cue.lpmama;
        await copyLpmamaContent(lpmama);
        setIsSmartCaptureProcessing(false);
        setPendingSmartCaptureAction(null);
        executeSmartCaptureAction(mode);
    }, [pendingSmartCaptureAction, executeSmartCaptureAction]);

    const handleSmartCaptureProceedAnyway = useCallback(() => {
        const mode = pendingSmartCaptureAction;
        if (!mode) return;
        setPendingSmartCaptureAction(null);
        executeSmartCaptureAction(mode);
    }, [pendingSmartCaptureAction, executeSmartCaptureAction]);

    const handleSmartCaptureDismiss = useCallback(() => {
        setPendingSmartCaptureAction(null);
    }, []);

    useEffect(() => {
        if (!isCoachActive && pendingSmartCaptureAction) {
            setPendingSmartCaptureAction(null);
        }
    }, [isCoachActive, pendingSmartCaptureAction]);

    useEffect(() => {
        if (!pendingSmartCaptureAction) return;
        const handleKeyDown = (e: KeyboardEvent) => {
            if (e.key === 'Escape' && !isSmartCaptureProcessing) setPendingSmartCaptureAction(null);
        };
        document.addEventListener('keydown', handleKeyDown);
        return () => document.removeEventListener('keydown', handleKeyDown);
    }, [pendingSmartCaptureAction, isSmartCaptureProcessing]);


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

        if (pendingSmartCaptureAction) {
            return getTotalHeightWithRef(smartCaptureWarningDialogRef) || WINDOW_HEIGHT_SIZES.BASE;
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
    }, [isDropdownOpen, currentInsight, leadType, insightsQueue, isCoachActive, coachFeature, isInsightsLayoutOpen, coachWindowError, showSessionAutoStopped, currentfs, lpmamaTooltipHeight, isZipCodeValid, isZipDropdownOpen, isPulseEnabled, allResults, pulseError, isPulsePending, pendingSmartCaptureAction, updateInsightsMaxListHeight]);

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
                Sentry.captureException(error);
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

            {isZipDropdownOpen && isZipCodeValid && isCoachActive && isPulseEnabled && !pendingSmartCaptureAction && (
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
            
            {isInsightsLayoutOpen && isCoachActive && !pendingSmartCaptureAction && (
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

            {pendingSmartCaptureAction && (
                <SmartCaptureWarningDialog
                    ref={smartCaptureWarningDialogRef}
                    mode={pendingSmartCaptureAction}
                    onPrimary={handleSmartCaptureCopyAndProceed}
                    onSecondary={handleSmartCaptureProceedAnyway}
                    onDismiss={handleSmartCaptureDismiss}
                    isProcessing={isSmartCaptureProcessing}
                />
            )}
        </div>
    );
}
