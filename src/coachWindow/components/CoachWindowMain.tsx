import { useEffect, useRef, useState, useCallback, useMemo } from 'react';
import { useSessionExpiry } from '@/hooks/useSessionExpiry';
import { MdDragIndicator } from 'react-icons/md';
import { MdErrorOutline } from 'react-icons/md';
import { LuX } from 'react-icons/lu';
import * as Sentry from "@sentry/electron/renderer";
import { useCoachWindowStore } from '../../store/coachWindowStore';
import CoachButtons from './CoachButtons';
import SelectProspectDropdown from './SelectProspectDropdown';
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
    //STATE
    const [isDropdownOpen, setIsDropdownOpen] = useState(false);
    const [isOffline, setIsOffline] = useState(false);
    const [showSessionAutoStopped, setShowSessionAutoStopped] = useState(false);
    const [lpmamaTooltipHeight, setLpmamaTooltipHeight] = useState(0);
    const [pendingSmartCaptureAction, setPendingSmartCaptureAction] = useState<SmartCaptureWarningMode | null>(null);
    const [isSmartCaptureProcessing, setIsSmartCaptureProcessing] = useState(false);
    //CONTEXT / HOOKS
    const sessionData = useCoachWindowStore(state => state.sessionData);
    const isCoachActive = useCoachWindowStore(state => state.isCoachActive);
    const coachFeature = useCoachWindowStore(state => state.coachFeature);
    const selectedProspect = useCoachWindowStore(state => state.recall.selectedProspect);
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
    usePlaybookPrefetch();
    const [zipCodeValue, setZipCodeValue] = useState<string>("")
    const [isZipDropdownOpen, setIsZipDropdownOpen] = useState<boolean>(false);

    const isZipCodeValid = useMemo(()=> {
        return /^\d{5}$/.test(zipCodeValue);
    },[zipCodeValue])

    const {
        selectedPropertyType,
        setSelectedPropertyType,
        valuesFound,
        pulseError,
        isPending: isPulsePending,
        fetch: fetchPulse,
    } = usePulseMarketProperty(zipCodeValue, sessionData?.sessionId ?? '');

    useEffect(() => {
        if (isZipCodeValid) setIsZipDropdownOpen(true);
    }, [isZipCodeValid]);
    
    const handleSessionExpired = useCallback(() => {
        useCoachWindowStore.setState({ error: 'Your session has expired. Please re-login from the main window.' });
        if (isCoachActive) handleStopCue().catch((err) => Sentry.captureException(err));
    }, [isCoachActive, handleStopCue]);
    useSessionExpiry(handleSessionExpired);

    useEffect(() => {
        const ipc = window.electron?.ipcRenderer;
        if (!ipc) return;
        const handleNetworkState = (state: unknown) => setIsOffline((state as string) === 'reconnecting');
        ipc.on('network:state-changed', handleNetworkState as any);
        ipc.invoke('network:get-state').then((s: unknown) => setIsOffline((s as string) === 'reconnecting')).catch(() => {});
        return () => { ipc.off('network:state-changed', handleNetworkState as any); };
    }, []);

    const executeSmartCaptureAction = useCallback((mode: SmartCaptureWarningMode) => {
        const action = mode === 'reset' ? onPressResetSession : handleStopCue;
        action().catch((err) => Sentry.captureException(err));
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
                height += insightsLayoutRef.current.offsetHeight + 6 + lpmamaTooltipHeight;
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
            // Use double requestAnimationFrame to ensure DOM has fully updated
            rafId = requestAnimationFrame(() => {
                requestAnimationFrame(() => {
                    updateWindowSize();
                });
            });
        }, delay);

        // Use ResizeObserver for automatic size updates
        const resizeObserver = new ResizeObserver(updateWindowSize);
        if (containerRef.current) {
            resizeObserver.observe(containerRef.current);
        }
        if (mainContainerRef.current) {
            resizeObserver.observe(mainContainerRef.current);
        }

        return () => {
            clearTimeout(timeoutId);
            if (rafId) cancelAnimationFrame(rafId);
            resizeObserver.disconnect();
        };
    }, [isDropdownOpen, currentInsight, leadType, insightsQueue, isCoachActive, coachFeature, isInsightsLayoutOpen, coachWindowError, showSessionAutoStopped, currentfs, lpmamaTooltipHeight, isZipCodeValid, isZipDropdownOpen, isPulseEnabled, valuesFound, pulseError, isPulsePending, pendingSmartCaptureAction]);

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
        recall: <SelectProspectDropdown isDropdownOpen={isDropdownOpen} setIsDropdownOpen={setIsDropdownOpen} />,
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
                        
                        {
                            (selectedProspect || leadType) && (
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

            {coachWindowError && coachFeature === 'cue' && leadType && (
                <div className='cue-error-container coach-box-bubble' ref={errorContainerRef}>
                    <MdErrorOutline className='cue-error-icon' />
                    <span className='cue-error-text'>
                        {isOffline
                            ? 'No internet connection — we\'ll reconnect automatically.'
                            : coachWindowError}
                    </span>
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
                    onClose={() => setIsZipDropdownOpen(false)}
                    zipCodeValue={zipCodeValue}
                    selectedPropertyType={selectedPropertyType}
                    setSelectedPropertyType={setSelectedPropertyType}
                    valuesFound={valuesFound}
                    pulseError={pulseError}
                    isPending={isPulsePending}
                    onFetch={fetchPulse}
                />
            )}
            
            {isInsightsLayoutOpen && isCoachActive && !pendingSmartCaptureAction && (
                <InsightsVerticalLayout
                    ref={insightsLayoutRef}
                    onLpmamaTooltipHeightChange={setLpmamaTooltipHeight}
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
