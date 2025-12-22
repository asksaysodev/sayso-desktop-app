import { useState, useEffect, useRef } from 'react';
import { useCoachWindowStore } from '../../store/coachWindowStore';
import calculateInsightDisplayDuration from '../helpers/calculateInsightDisplayDuration';
import { LuX } from 'react-icons/lu';
import '../styles/InsightVerticalLayout.css';

const TIMING_CONFIG = {
    displayDuration: 6000,
    transitionDelay: 500,
    animationDuration: 300,
    enterDelay: 10,
};
const MOCKED = [
        {
            "message": "It's completely okay to take a pause. What feelings or goals are driving this uncertainty for you? I'm here to help clarify your thoughts.",
            "priority": 6,
            "appointmentBooked": false,
            "createdAt": 1766430529133
        },
        {
            "message": "I hear you on wanting to wait and see. Are there specific time frames you have in mind for when you'd feel more ready to explore options? I'm here to help you navigate this whenever you're ready.",
            "priority": 4,
            "appointmentBooked": false,
            "createdAt": 1766430534612
        }
]
export default function InsightsVerticalLayout({ isInsightsLayoutOpen, setIsInsightsLayoutOpen }) {
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

        if (currentQueueLength > previousQueueLength) {
            const newItems = new Set();
            
            for (let i = previousQueueLength; i < currentQueueLength; i++) {
                if (insightsQueue[i]) {
                    const itemId = `${insightsQueue[i].message}_${i}`;
                    newItems.add(itemId);
                }
            }
            
            setNewQueueItems(newItems);
            
            const cleanupTimer = setTimeout(() => {
                setNewQueueItems(new Set());
            }, TIMING_CONFIG.animationDuration + 100);

            previousQueueLengthRef.current = currentQueueLength;

            return () => clearTimeout(cleanupTimer);
        } else {
            previousQueueLengthRef.current = currentQueueLength;
        }
    }, [insightsQueue]);

    useEffect(() => {
        if (!currentInsight) {
            return;
        }

        const insightId = currentInsight.message;

        if (!insightStates[insightId]) {
            setInsightStates(prev => ({
                ...prev,
                [insightId]: { isVisible: false, isExiting: false }
            }));
        }

        const calculatedDisplayDuration = calculateInsightDisplayDuration(currentInsight.message);
        const finalDisplayDuration = calculatedDisplayDuration || TIMING_CONFIG.displayDuration;

        const enterTimer = setTimeout(() => {
            setInsightStates(prev => ({
                ...prev,
                [insightId]: { ...prev[insightId], isVisible: true }
            }));
        }, TIMING_CONFIG.enterDelay);

        const exitTimer = setTimeout(() => {
            setInsightStates(prev => ({
                ...prev,
                [insightId]: { ...prev[insightId], isExiting: true }
            }));
        }, finalDisplayDuration - TIMING_CONFIG.animationDuration);

        const completeTimer = setTimeout(() => {
            showNext();
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

    console.log(isInsightsLayoutOpen, 'isInsightsLayoutOpen', allInsightsToDisplay)
    if (!isInsightsLayoutOpen) return null;
    return (
        <div className="insights-vertical-layout-container">
            {MOCKED.length > 0 
            ? (
                <ul className="insights-vertical-list">
                {MOCKED.map((insight, index) => {
                    const isFirstInsight = index === 0;
                    const insightId = insight.message;
                    
                    const insightState = insightStates[insightId] || { isVisible: false, isExiting: false };
                    
                    const queueIndex = index - 1;
                    const itemId = `${insight.message}_${queueIndex}`;
                    const isNewQueueItem = !isFirstInsight && newQueueItems.has(itemId);
                    
                    return (
                        <li
                            key={insight.message + index}
                            // className={`
                            //     insights-vertical-layout-item
                            //     ${isFirstInsight ? 'insights-vertical-layout-item--active' : 'insights-vertical-layout-item--queue'}
                            //     ${isNewQueueItem ? 'insights-vertical-layout-item--new' : ''}
                            // `}
                            className={`
                                insights-vertical-layout-item
                                ${isFirstInsight ? 'insights-vertical-layout-item--active' : 'insights-vertical-layout-item--queue'}
                            `}
                            // style={{
                            //     '--animation-duration': `${TIMING_CONFIG.animationDuration}ms`,
                            // }}
                        >
                            <button className="insights-vertical-layout-item__close-button">
                                <LuX size={12}/>
                            </button>
                            <div className="insights-vertical-layout-item__text">
                                {insight.message}
                            </div>
                        </li>
                    );
                })}
            </ul>
            ) : (
                <div className="no-insights-to-display-container">
                    <p>No insights to display</p>
                </div>
            )}
        </div>
    );
}