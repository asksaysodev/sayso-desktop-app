import { useEffect, useRef, useState, useCallback, MouseEventHandler } from 'react';
import { useSessionExpiry } from '@/hooks/useSessionExpiry';

import { MdDragIndicator } from 'react-icons/md';
import { IoClose } from 'react-icons/io5';
import { MdErrorOutline } from 'react-icons/md';
import { LuSettings, LuX } from 'react-icons/lu';
import * as Sentry from "@sentry/electron/renderer";
import { useCoachWindowStore } from '../../store/coachWindowStore';

import CoachButtons from './CoachButtons';
import SelectProspectDropdown from './SelectProspectDropdown';
import SelectLeadTypeDropdown from './SelectLeadTypeDropdown';
import InsightsVerticalLayout from './InsightsVerticalLayout';
import SessionStoppedDialog from './SessionStoppedDialog';
import useFontSize from '../hooks/useFontSize';

const WINDOW_WIDTH_SIZES = {
    s: {
        BASE: 380,
        READY_TO_LAUNCH: 400,
        ACTIVE_SESSION: 440,
        MAX_WIDTH: 900
    },
    m: {
        BASE: 400,
        READY_TO_LAUNCH: 440,
        ACTIVE_SESSION: 460,
        MAX_WIDTH: 900
    },
    l: {
        BASE: 420,
        READY_TO_LAUNCH: 448,
        ACTIVE_SESSION: 500,
        MAX_WIDTH: 900
    },
}

const WINDOW_HEIGHT_SIZES = {
    BASE: 54,
    DROPDOWN_OPEN: 220,
    ERROR: 110,
}

type DragStartRef = { mouseX: number, mouseY: number, winX: number, winY: number };

export default function CoachWindowMain() {

    //REFS
    const containerRef = useRef<HTMLDivElement | null>(null);
    const insightsLayoutRef = useRef<HTMLDivElement | null>(null);
    const errorContainerRef = useRef<HTMLDivElement | null>(null);
    const sessionStoppedDialogRef = useRef<HTMLDivElement | null>(null);
    const isDraggingRef = useRef(false);
    const dragStartRef = useRef<DragStartRef>({ mouseX: 0, mouseY: 0, winX: 0, winY: 0 });
    //STATE
    const [isSmartCaptureActive, setIsSmartCaptureActive] = useState(false);
    const [isDropdownOpen, setIsDropdownOpen] = useState(false);
    const [showSessionAutoStopped, setShowSessionAutoStopped] = useState(false);
    const [isDragging, setIsDragging] = useState(false);
    const [isCoachSettingsWindowOpen, setIsCoachSettingsWindowOpen] = useState(false);
    //CONTEXT / HOOKS
    const isCoachActive = useCoachWindowStore(state => state.isCoachActive);
    const coachFeature = useCoachWindowStore(state => state.coachFeature);
    const selectedProspect = useCoachWindowStore(state => state.recall.selectedProspect);
    const leadType = useCoachWindowStore(state => state.cue.leadType);
    const currentInsight = useCoachWindowStore(state => state.cue.currentInsight);
    const insightsQueue = useCoachWindowStore(state => state.cue.insightsQueue);
    const closeCoachWindow = useCoachWindowStore(state => state.closeCoachWindow);
    const isInsightsLayoutOpen = useCoachWindowStore(state => state.cue.isInsightsLayoutOpen);
    const setIsInsightsLayoutOpen = useCoachWindowStore(state => state.cue_setIsInsightsLayoutOpen);
    const coachWindowError = useCoachWindowStore(state => state.error);
    const clearError = useCoachWindowStore(state => state.clearError);
    const hasReceivedFirstInsight = useCoachWindowStore(state => state.cue.hasReceivedFirstInsight);
    const setHasReceivedFirstInsight = useCoachWindowStore(state => state.cue_setHasReceivedFirstInsight);
    const incrementUnseenInsightsCount = useCoachWindowStore(state => state.cue_incrementUnseenInsightsCount);
    const handleStopCue = useCoachWindowStore(state => state.cue_handleStopCue);
    const currentfs = useFontSize();

    const handleCloseCoachWindow = () => {
        closeCoachWindow()
    }

    const handleSessionExpired = useCallback(() => {
        useCoachWindowStore.setState({ error: 'Your session has expired. Please re-login from the main window.' });
        if (isCoachActive) handleStopCue().catch((err) => Sentry.captureException(err));
    }, [isCoachActive, handleStopCue]);
    useSessionExpiry(handleSessionExpired);

    // Manual window drag handlers
    const handleDragStart = async (e: MouseEvent) => {
        e.preventDefault();
        isDraggingRef.current = true;
        setIsDragging(true);

        if(!window.electronAPI) return;

        const [winX, winY] = await window.electronAPI.getWindowPosition();
        dragStartRef.current = {
            mouseX: e.screenX,
            mouseY: e.screenY,
            winX,
            winY
        };

        document.addEventListener('mousemove', handleDragMove);
        document.addEventListener('mouseup', handleDragEnd);
    };

    const handleDragMove = (e: MouseEvent) => {
        if (!isDraggingRef.current) return;

        const deltaX = e.screenX - dragStartRef.current.mouseX;
        const deltaY = e.screenY - dragStartRef.current.mouseY;

        const newX = dragStartRef.current.winX + deltaX;
        const newY = dragStartRef.current.winY + deltaY;

        if(!window.electronAPI) return;

        window.electronAPI.setWindowPosition(newX, newY);
    };

    const handleDragEnd = () => {
        isDraggingRef.current = false;
        setIsDragging(false);
        document.removeEventListener('mousemove', handleDragMove);
        document.removeEventListener('mouseup', handleDragEnd);
    };

    function getTotalHeightWithRef(ref: any) {
        if (!ref || !ref?.current) return 0;

        const actualHeight = ref.current?.offsetHeight ?? 0;
        const totalHeight = WINDOW_HEIGHT_SIZES.BASE + actualHeight + 6;
        return totalHeight; 
    }
    
    function getWidthByCurrentState() {
        const fsWidthOptions = WINDOW_WIDTH_SIZES[currentfs];
        if (isCoachActive && coachFeature === 'cue') return fsWidthOptions.ACTIVE_SESSION;
        if (leadType !== null) return fsWidthOptions.READY_TO_LAUNCH;
        return fsWidthOptions.BASE;
    }

    function getHeightByCurrentState() {
        const showError = coachWindowError && coachFeature === 'cue' && leadType;
        if (showError) {
            return getTotalHeightWithRef(errorContainerRef);
        }

        if (isDropdownOpen) {
            return WINDOW_HEIGHT_SIZES.DROPDOWN_OPEN;
        } else if (isInsightsLayoutOpen) {
            if (insightsLayoutRef.current) {
                return getTotalHeightWithRef(insightsLayoutRef);
            }
            
            const queueLength = insightsQueue.length;
            if (queueLength === 0) return 280;
            if (queueLength === 1) return 140;
            if (queueLength === 2) return 210;
            if (queueLength === 3) return 280;
            if (queueLength >= 4) return 350;
            
            return 280;
        } else if (showSessionAutoStopped) {
            return getTotalHeightWithRef(sessionStoppedDialogRef) || WINDOW_HEIGHT_SIZES.BASE;
        }
        return WINDOW_HEIGHT_SIZES.BASE;
    }

    useEffect(() => {
        let isResizing = false;

        const updateWindowSize = () => {
            if (containerRef.current && window.electronAPI && !isResizing) {
                isResizing = true;
                const fsWidthOptions = WINDOW_WIDTH_SIZES[currentfs];
                const windowWidth = Math.max(
                    fsWidthOptions.BASE, 
                    Math.min(fsWidthOptions.MAX_WIDTH, getWidthByCurrentState())
                );
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

        return () => {
            clearTimeout(timeoutId);
            if (rafId) cancelAnimationFrame(rafId);
            resizeObserver.disconnect();
        };
    }, [isDropdownOpen, currentInsight, leadType, insightsQueue, isCoachActive, coachFeature, isInsightsLayoutOpen, coachWindowError, showSessionAutoStopped, currentfs]);

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
      const ipcRenderer = window.electron?.ipcRenderer;
      
      if (!ipcRenderer) return;
  
      const handleCoachSettingsWindowState = (state: { isOpen: boolean }) => {
        setIsCoachSettingsWindowOpen(state.isOpen);
      };
  
      ipcRenderer.on('coach-settings-window-state', handleCoachSettingsWindowState); // $FixTS
      ipcRenderer.send('get-coach-settings-window-state');

      return () => {
        ipcRenderer.off('coach-settings-window-state', handleCoachSettingsWindowState); // $FixTS
      };
    }, []);
    
    const openCoachSettingsWindow = () => {
        const ipcRenderer = window.electron?.ipcRenderer;

        if (ipcRenderer) {
            if (isCoachSettingsWindowOpen) {
                ipcRenderer.send('close-coach-settings-window');
                setIsCoachSettingsWindowOpen(false);
            } else {
                ipcRenderer.send('open-coach-settings-window');
                setIsCoachSettingsWindowOpen(true);
            }
        }
    }
    
    return (
        <div className="coach-window" ref={containerRef}>
            <div className={`main-container coach-box-bubble`}>
                <div className='main-toolbar'>
                    <div className="coach-window-drag-container">
                        <button
                            className='coach-window-drag-indicator'
                            onMouseDown={handleDragStart as unknown as MouseEventHandler<HTMLButtonElement>}
                        >
                            <MdDragIndicator/>
                        </button>
                        <div className="coach-window-divider"></div>
                    </div>
                    
                    <div className='inner-content-centered'>
                        {
                            DropdownComponent[coachFeature] && (
                                DropdownComponent[coachFeature]
                            )
                        }
    
                        {
                            (selectedProspect || leadType) && (
                                <CoachButtons 
                                    isInsightsLayoutOpen={isInsightsLayoutOpen}
                                    setIsInsightsLayoutOpen={setIsInsightsLayoutOpen} 
                                    setIsDropdownOpen={setIsDropdownOpen}
                                    isDropdownOpen={isDropdownOpen}
                                />
                            )
                        }
                    </div>

                    {
                        !isCoachActive && (
                            <>
                                <button className='right-side-coach-button' onClick={openCoachSettingsWindow}>
                                    <LuSettings />
                                </button>
                                <button className='right-side-coach-button' onClick={() => handleCloseCoachWindow()}>
                                    <IoClose />
                                </button>
                            </>
                        )
                    }
                </div>
            </div>

            {coachWindowError && coachFeature === 'cue' && leadType && (
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

            {isInsightsLayoutOpen && isCoachActive && (
                <InsightsVerticalLayout 
                    ref={insightsLayoutRef}
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
