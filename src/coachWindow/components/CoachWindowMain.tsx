import { useEffect, useRef, useState, useCallback, useMemo, MouseEventHandler } from 'react';

import { MdDragIndicator } from 'react-icons/md';
import { IoClose } from 'react-icons/io5';
import { MdErrorOutline } from 'react-icons/md';
import { LuSettings, LuX } from 'react-icons/lu';
import * as Sentry from "@sentry/electron/renderer";
import { useCoachWindowStore } from '../../store/coachWindowStore';

import CoachButtons from './CoachButtons';
import InsightWrapper from './InsightWrapper';
import SelectProspectDropdown from './SelectProspectDropdown';
import SelectLeadTypeDropdown from './SelectLeadTypeDropdown';
import InsightsVerticalLayout from './InsightsVerticalLayout';
import SessionStoppedDialog from './SessionStoppedDialog';

const WINDOW_WIDTH_SIZES = {
    BASE: 320,
    READY_TO_LAUNCH: 400,
    ACTIVE_SESSION: 440,
    MAX_WIDTH: 900
}

const WINDOW_HEIGHT_SIZES = {
    BASE: 54,
    DROPDOWN_OPEN: 220,
    ERROR: 110,
}

// cue
// const defaultConfig = {
//     /** Display duration of the toast */
//     displayDuration: 6000,
//     /** Time between toasts - allows exit animation (300ms) to complete with buffer */
//     transitionDelay: 500,
//     /** Time until the toast expires */
//     expirationTime: 30000,
//     /** Animation duration of the toast */
//     animationDuration: 300,
//     /** Time until the toast is considered too old to display */
//     maxAgeBeforeDisplay: 90000, // 90s
// };

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
    const isCueDisplaying = useCoachWindowStore(state => state.cue.isInsightDisplaying);
    const showNext = useCoachWindowStore(state => state.cue_showNext);
    const closeCoachWindow = useCoachWindowStore(state => state.closeCoachWindow);
    const isInsightsLayoutOpen = useCoachWindowStore(state => state.cue.isInsightsLayoutOpen);
    const setIsInsightsLayoutOpen = useCoachWindowStore(state => state.cue_setIsInsightsLayoutOpen);
    const coachWindowError = useCoachWindowStore(state => state.error);
    const clearError = useCoachWindowStore(state => state.clearError);
    const hasReceivedFirstInsight = useCoachWindowStore(state => state.cue.hasReceivedFirstInsight);
    const setHasReceivedFirstInsight = useCoachWindowStore(state => state.cue_setHasReceivedFirstInsight);
    const incrementUnseenInsightsCount = useCoachWindowStore(state => state.cue_incrementUnseenInsightsCount);
    const handleStopCue = useCoachWindowStore(state => state.cue_handleStopCue);

    const handleCloseCoachWindow = () => {
        closeCoachWindow()
    }

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
        if (isCoachActive && coachFeature === 'cue') return WINDOW_WIDTH_SIZES.ACTIVE_SESSION;
        if (leadType !== null) return WINDOW_WIDTH_SIZES.READY_TO_LAUNCH;
        return WINDOW_WIDTH_SIZES.BASE;
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

                const windowWidth = Math.max(
                    WINDOW_WIDTH_SIZES.BASE, 
                    Math.min(WINDOW_WIDTH_SIZES.MAX_WIDTH, getWidthByCurrentState())
                );
                
                const windowHeight = getHeightByCurrentState();

                console.log('🔍 [updateWindowSize]', {
                    currentInsight: currentInsight !== null ? 'exists' : 'null',
                    insightsQueueLength: insightsQueue.length,
                    windowWidth,
                    windowHeight,
                    isDropdownOpen,
                    isInsightsLayoutOpen,
                    containerOffsetHeight: containerRef.current.offsetHeight
                });

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
    }, [isDropdownOpen, currentInsight, leadType, insightsQueue, isCoachActive, coachFeature, isInsightsLayoutOpen, coachWindowError, showSessionAutoStopped]);

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
			console.log('🎯 [New Insight Received]', {
				message: insightData.message?.substring(0, 50) + '...',
				priority: insightData.priority,
				appointmentBooked: insightData.appointmentBooked
			});

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
        if (!window.electron?.cue?.onAutoStop) {
            console.warn('⚠️ [CoachWindowMain] Electron cue.onAutoStop not available');
            return;
        }

        const unsubscribe = window.electron.cue.onAutoStop(async () => {
            console.log('🔴 [Auto Stop Received], stopping cue');
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
