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

    //CONTEXT / HOOKS
    const isCoachActive = useCoachWindowStore(state => state.isCoachActive);
    const coachFeature = useCoachWindowStore(state => state.coachFeature);

    const selectedProspect = useCoachWindowStore(state => state.recall.selectedProspect);
    const leadType = useCoachWindowStore(state => state.cue.leadType);
    const currentInsight = useCoachWindowStore(state => state.cue.currentInsight);
    const insightsQueue = useCoachWindowStore(state => state.cue.insightsQueue);
    const isCueDisplaying = useCoachWindowStore(state => state.cue.isDisplaying);
    const showNext = useCoachWindowStore(state => state.cue_showNext);

    const handleCloseCoachWindow = () => {
        console.log('🎯 [CoachWindowMain] Close button clicked, closing window directly');
        if(window.electronAPI) {
            window.electronAPI.closeCoachWindow()
        }
    }

    useEffect(() => {
        let isResizing = false; // Flag to prevent feedback loop
        
        const updateWindowSize = () => {
            if (containerRef.current && window.electronAPI && !isResizing) {
                isResizing = true; // Set flag to prevent recursive calls
                
                const contentHeight = containerRef.current.scrollHeight;
                const contentWidth = containerRef.current.scrollWidth;
                
                // Calculate responsive dimensions
                const windowHeight = Math.max(48, (isDropdownOpen ? 298 : contentHeight + 8 ))
                
                // Width based on content with min/max constraints
                const minWidth = 300;  // Minimum usable width
                const maxWidth = 1200; // Maximum width before it gets too wide
                const padding = 20;    // Padding around content
                
                // Add 700px to width when displayInsight is true
                // const insightWidth = currentInsight ? 700 : 0;
                const insightWidth = 0;
                const windowWidth = Math.max(minWidth, Math.min(maxWidth, contentWidth + padding + insightWidth));
                
                window.electronAPI.resizeWindow(windowWidth, windowHeight);
                
                // Reset flag after a short delay
                setTimeout(() => {
                    isResizing = false;
                }, 100);
            }
        };

        // Update size when content changes
        updateWindowSize();
        
        // Use ResizeObserver for automatic size updates
        const resizeObserver = new ResizeObserver(updateWindowSize);
        if (containerRef.current) {
            resizeObserver.observe(containerRef.current);
        }

        return () => resizeObserver.disconnect();
    }, [isDropdownOpen]);

    const onCompleteInsight = useCallback(() => {
        showNext();
    }, [showNext]);

     // Auto-trigger showNext when queue has items and nothing is currently displaying
     useEffect(() => {
        if (insightsQueue.length > 0 && !isCueDisplaying && !currentInsight) {
            showNext();
        }
    }, [insightsQueue, isCueDisplaying, currentInsight]);

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
