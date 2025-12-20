import { useState, useEffect, useRef } from 'react';
import { useCoachWindowStore } from '../../store/coachWindowStore';
import calculateInsightDisplayDuration from '../helpers/calculateInsightDisplayDuration';
import '../styles/InsightVerticalLayout.css';

const TIMING_CONFIG = {
    displayDuration: 6000,
    transitionDelay: 500,
    animationDuration: 300,
    enterDelay: 10,
};

export default function InsightsVerticalLayout() {
    const currentInsight = useCoachWindowStore(state => state.cue.currentInsight);
    const insightsQueue = useCoachWindowStore(state => state.cue.insightsQueue);
    const showNext = useCoachWindowStore(state => state.cue_showNext);
    
    const [insightStates, setInsightStates] = useState({});
    const [newQueueItems, setNewQueueItems] = useState(new Set());
    const previousQueueLengthRef = useRef(0);
    
    const upcomingInsights = insightsQueue.slice(0, 4);
    const allInsightsToDisplay = currentInsight 
        ? [currentInsight, ...upcomingInsights]
        : [];

    /**
     * Detect when new items are added to the queue and mark them for entrance animation
     * This creates a smooth UX when insights arrive while others are displaying
     */
    useEffect(() => {
        const currentQueueLength = insightsQueue.length;
        const previousQueueLength = previousQueueLengthRef.current;

        // New item(s) added to the queue
        if (currentQueueLength > previousQueueLength) {
            const newItems = new Set();
            
            // Mark new items at the end of the queue
            for (let i = previousQueueLength; i < currentQueueLength; i++) {
                if (insightsQueue[i]) {
                    // Use message + timestamp as unique identifier
                    const itemId = `${insightsQueue[i].message}_${i}`;
                    newItems.add(itemId);
                }
            }
            
            setNewQueueItems(newItems);
            
            // Remove the "new" status after animation completes
            const cleanupTimer = setTimeout(() => {
                setNewQueueItems(new Set());
            }, TIMING_CONFIG.animationDuration + 100);

            previousQueueLengthRef.current = currentQueueLength;

            return () => clearTimeout(cleanupTimer);
        } else {
            previousQueueLengthRef.current = currentQueueLength;
        }
    }, [insightsQueue]);

    /** 
     * Handle timing for the current (first) insight following InsightWrapper rules
     * Each insight gets its own animation state to prevent race conditions
     */
    useEffect(() => {
        if (!currentInsight) {
            return;
        }

        const insightId = currentInsight.message;

        // Initialize state for this insight if it doesn't exist
        if (!insightStates[insightId]) {
            setInsightStates(prev => ({
                ...prev,
                [insightId]: { isVisible: false, isExiting: false }
            }));
        }

        // Calculate display duration based on text length
        const calculatedDisplayDuration = calculateInsightDisplayDuration(currentInsight.message);
        const finalDisplayDuration = calculatedDisplayDuration || TIMING_CONFIG.displayDuration;

        // Enter animation: show the insight after a small delay
        const enterTimer = setTimeout(() => {
            setInsightStates(prev => ({
                ...prev,
                [insightId]: { ...prev[insightId], isVisible: true }
            }));
        }, TIMING_CONFIG.enterDelay);

        // Exit animation: start exit animation before the insight completes
        const exitTimer = setTimeout(() => {
            setInsightStates(prev => ({
                ...prev,
                [insightId]: { ...prev[insightId], isExiting: true }
            }));
        }, finalDisplayDuration - TIMING_CONFIG.animationDuration);

        // Complete: call showNext() to move to the next insight in the queue
        const completeTimer = setTimeout(() => {
            showNext();
            // Clean up this insight's state after transition completes
            setTimeout(() => {
                setInsightStates(prev => {
                    const newStates = { ...prev };
                    delete newStates[insightId];
                    return newStates;
                });
            }, TIMING_CONFIG.transitionDelay + 100);
        }, finalDisplayDuration + TIMING_CONFIG.transitionDelay);

        return () => {
            clearTimeout(enterTimer);
            clearTimeout(exitTimer);
            clearTimeout(completeTimer);
        };
    }, [currentInsight, showNext]);

    if (allInsightsToDisplay.length === 0) {
        return null;
    }

    return (
        <div className="insights-vertical-layout-container">
            <ul className="insights-vertical-list">
                {allInsightsToDisplay.map((insight, index) => {
                    const isFirstInsight = index === 0;
                    const insightId = insight.message;
                    
                    const insightState = insightStates[insightId] || { isVisible: false, isExiting: false };
                    
                    const queueIndex = index - 1;
                    const itemId = `${insight.message}_${queueIndex}`;
                    const isNewQueueItem = !isFirstInsight && newQueueItems.has(itemId);
                    
                    return (
                        <li
                            key={insight.message + index}
                            className={`
                                insights-vertical-layout-item
                                ${isFirstInsight ? 'insights-vertical-layout-item--active' : 'insights-vertical-layout-item--queue'}
                                ${isFirstInsight && insightState.isVisible ? 'insights-vertical-layout-item--visible' : ''}
                                ${isFirstInsight && insightState.isExiting ? 'insights-vertical-layout-item--exiting' : ''}
                                ${isNewQueueItem ? 'insights-vertical-layout-item--new' : ''}
                            `}
                            style={{
                                '--animation-duration': `${TIMING_CONFIG.animationDuration}ms`,
                            }}
                        >
                            <div className="insights-vertical-layout-item__text">
                                {insight.message}
                            </div>
                        </li>
                    );
                })}
            </ul>
        </div>
    );
}