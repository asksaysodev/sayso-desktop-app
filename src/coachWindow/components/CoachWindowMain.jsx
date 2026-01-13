import { useEffect, useRef, useState, useCallback, useMemo } from 'react';

import { MdDragIndicator } from 'react-icons/md';
import { IoClose } from 'react-icons/io5';
import { MdErrorOutline } from 'react-icons/md';
import { LuX } from 'react-icons/lu';

import { useCoachWindowStore } from '../../store/coachWindowStore';

import CoachButtons from './CoachButtons';
import InsightWrapper from './InsightWrapper';
import SelectProspectDropdown from './SelectProspectDropdown';
import SelectLeadTypeDropdown from './SelectLeadTypeDropdown';
import InsightsVerticalLayout from './InsightsVerticalLayout';

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
const defaultConfig = {
    /** Display duration of the toast */
    displayDuration: 6000,
    /** Time between toasts - allows exit animation (300ms) to complete with buffer */
    transitionDelay: 500,
    /** Time until the toast expires */
    expirationTime: 30000,
    /** Animation duration of the toast */
    animationDuration: 300,
    /** Time until the toast is considered too old to display */
    maxAgeBeforeDisplay: 90000, // 90s
};

export default function CoachWindowMain() {

    //REFS
    const containerRef = useRef(null);
    const insightsLayoutRef = useRef(null);
    const errorContainerRef = useRef(null);

    //STATE
    const [isSmartCaptureActive, setIsSmartCaptureActive] = useState(false);
    const [isDropdownOpen, setIsDropdownOpen] = useState(false);

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

    const insightsLayoutOpen = useMemo(() => isInsightsLayoutOpen && insightsQueue.length > 0 && coachFeature ==='cue' && leadType && isCoachActive, [isInsightsLayoutOpen, insightsQueue, coachFeature, leadType, isCoachActive]);

    const handleCloseCoachWindow = () => {
        closeCoachWindow()
    }

    function getTotalHeightWithRef(ref) {
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

        let rafId = null;
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
    }, [isDropdownOpen, currentInsight, leadType, insightsQueue, isCoachActive, coachFeature, isInsightsLayoutOpen, coachWindowError]);

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

            if (!isInsightsLayoutOpen) {
                setIsInsightsLayoutOpen(true);
            }
		});

		return () => {
			unsubscribe();
		};
	}, [isCoachActive, coachFeature, isInsightsLayoutOpen]);

    const DropdownComponent = {
        recall: <SelectProspectDropdown isDropdownOpen={isDropdownOpen} setIsDropdownOpen={setIsDropdownOpen} />,
        cue: <SelectLeadTypeDropdown isDropdownOpen={isDropdownOpen} setIsDropdownOpen={setIsDropdownOpen} />,
    };

    return (
        <div className="coach-window" ref={containerRef}>
            <div className={`main-container coach-box-bubble`}>
                <div className='main-toolbar'>
                    <div className="coach-window-drag-container">
                        <MdDragIndicator/>
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
                            <button className='close-coach-button-container' onClick={() => handleCloseCoachWindow()}>
                                <IoClose />
                            </button>
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

            {insightsLayoutOpen && (
                <InsightsVerticalLayout 
                    ref={insightsLayoutRef}
                    setIsInsightsLayoutOpen={setIsInsightsLayoutOpen} 
                />
            )}
        </div>
    );
}
