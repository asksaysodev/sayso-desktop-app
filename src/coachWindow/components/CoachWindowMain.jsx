import { useEffect, useRef, useState, useCallback } from 'react';

import { MdDragIndicator } from 'react-icons/md';
import { IoClose } from 'react-icons/io5';

import { useCoachWindowStore } from '../../store/coachWindowStore';

import CoachButtons from './CoachButtons';
import InsightWrapper from './InsightWrapper';
import SelectProspectDropdown from './SelectProspectDropdown';
import SelectLeadTypeDropdown from './SelectLeadTypeDropdown';

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

    //STATE
    const [isSmartCaptureActive, setIsSmartCaptureActive] = useState(false);
    const [isDropdownOpen, setIsDropdownOpen] = useState(false);
	const [isInsightsWindowOpen, setIsInsightsWindowOpen] = useState(false);

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

    const handleCloseCoachWindow = () => {
        closeCoachWindow()
    }

    useEffect(() => {
        let isResizing = false; // Flag to prevent feedback loop
        
        const updateWindowSize = () => {
            if (containerRef.current && window.electronAPI && !isResizing) {
                isResizing = true; // Set flag to prevent recursive calls
                
                const contentHeight = containerRef.current.scrollHeight;
                const contentWidth = containerRef.current.scrollWidth;
                
                // Calculate responsive dimensions
                const windowHeight = Math.max(45, (isDropdownOpen ? 220 : contentHeight ))
                
                // Width based on content with min/max constraints
                const minWidth = 300;  // Minimum usable width
                const maxWidth = 1200; // Maximum width before it gets too wide
                
                // Only track currentInsight - it's the source of truth for what's displayed
                // When showNext() runs, it atomically updates currentInsight and clears the queue if empty
                // So if currentInsight is null, there's nothing displayed (even if queue has items, they'll become currentInsight immediately)
                // Note: CSS transforms don't affect scrollWidth, so we manually add the insight width when present
                const hasInsight = currentInsight !== null;
                const insightWidth = hasInsight ? 395 : 0;
                
                // contentWidth is the base width (doesn't include insight due to CSS transforms)
                // Simply add insightWidth when an insight is present
                const windowWidth = Math.max(minWidth, Math.min(maxWidth, contentWidth + insightWidth));
                
                console.log('🔍 [updateWindowSize]', {
                    currentInsight: currentInsight !== null ? 'exists' : 'null',
                    insightsQueueLength: insightsQueue.length,
                    hasInsight,
                    contentWidth,
                    insightWidth,
                    windowWidth,
                    windowHeight,
                    isDropdownOpen
                });
                
                window.electronAPI.resizeWindow(windowWidth, windowHeight);
                
                // Reset flag after a short delay
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
    }, [isDropdownOpen, currentInsight]); // Only track currentInsight for width - it's the source of truth

    const onCompleteInsight = useCallback(() => {
        showNext();
    }, [showNext]);

	useEffect(() => {
		console.log('📊 [Insight State Change]', {
			currentInsight: currentInsight !== null ? 'exists' : 'null',
			insightsQueueLength: insightsQueue.length,
			isInsightsWindowOpen
		});

		// When currentInsight appears, set to true immediately
		if (currentInsight !== null) {
			console.log('✅ [Insight] Setting isInsightsWindowOpen to true (insight appeared)');
			setIsInsightsWindowOpen(true);
			return;
		}

		// When currentInsight disappears
		if (currentInsight === null) {
			// If there are queued insights, keep it open (they'll show next)
			if (insightsQueue.length > 0) {
				console.log('⏳ [Insight] Keeping isInsightsWindowOpen true (queue has items)');
				setIsInsightsWindowOpen(true);
				return;
			}

			// Otherwise, delay closing to allow exit animation to complete
			// Keep window expanded for displayDuration + transitionDelay to ensure smooth transition
			const closeDelay = defaultConfig.displayDuration + defaultConfig.transitionDelay;
			console.log(`⏰ [Insight] Scheduling close in ${closeDelay}ms (no insights)`);
			const timeoutId = setTimeout(() => {
				console.log('❌ [Insight] Setting isInsightsWindowOpen to false (delayed close)');
				setIsInsightsWindowOpen(false);
			}, closeDelay);

			return () => clearTimeout(timeoutId);
		}
	}, [currentInsight, insightsQueue.length]);

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
			
			// Call store action to add insight
			const addInsight = useCoachWindowStore.getState().cue_addInsight;
			
			addInsight({
				message: insightData.message,
				priority: insightData.priority,
				appointmentBooked: insightData.appointmentBooked || false,
			});
		});
		
		return () => {
			unsubscribe();
		};
	}, [isCoachActive, coachFeature]);

    // Auto-trigger showNext when queue has items and nothing is currently displaying
	useEffect(() => {
		if (insightsQueue.length > 0 && !isCueDisplaying && !currentInsight) {
			showNext();
		}
    }, [insightsQueue, isCueDisplaying, currentInsight, showNext]);

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
                    </div>
                    {/* <div className="coach-window-divider"></div>
                    <div className='coach-smart-capture-toggle-container' onClick={handleSmartCapture}>
                        <p>Show Smart Capture</p>
                        <div className='coach-smart-capture-toggle' data-active={isSmartCaptureActive} >
                            <span></span>
                        </div>
                    </div> */}
                    <div className="coach-window-divider"></div>
                    { 
                        DropdownComponent[coachFeature] && (
                            DropdownComponent[coachFeature]
                        )
                    }

                    {
                        (selectedProspect || leadType) && (
                            <CoachButtons />
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
                {/* {
                    isSmartCaptureActive && (
                        <SmartCaptureBox signals={signals} />
                    )
                } */}
            </div>
            
            {currentInsight && isCoachActive && (
                <InsightWrapper 
                    onComplete={onCompleteInsight}
                    priority={currentInsight?.priority}
                    insightText={currentInsight?.message}
                    displayDuration={defaultConfig.displayDuration}
                    transitionDelay={defaultConfig.transitionDelay}
                    animationDuration={defaultConfig.animationDuration}
                />
            )}
        </div>
    );
}
