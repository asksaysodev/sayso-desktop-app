import { useState, useEffect, useRef, useMemo, forwardRef } from 'react';
import { useCoachWindowStore } from '../../store/coachWindowStore';
import { LuX } from 'react-icons/lu';
import LpmamaRow from './LpmamaRow';
import '../styles/InsightVerticalLayout.css';

const TIMING_CONFIG = {
    entranceAnimationDuration: 400,
    exitAnimationDuration: 250,
};

interface Props {
    onLpmamaTooltipHeightChange: (height: number) => void;
}

const InsightsVerticalLayout = forwardRef<HTMLDivElement, Props>(({ onLpmamaTooltipHeightChange }, ref) => {
    const insightsQueue = useCoachWindowStore(state => state.cue.insightsQueue);
    const removeInsight = useCoachWindowStore(state => state.cue_removeInsight);
    const isSmartCaptureEnabled = useCoachWindowStore(state => state.cue.enabledFeatures.includes('smart_capture'));

    const [enteringItems, setEnteringItems] = useState(new Set());
    const [exitingItems, setExitingItems] = useState(new Set());
    const previousInsightsRef = useRef(new Map());

    const allInsightsToDisplay = useMemo(() => insightsQueue.slice(0, 5), [insightsQueue]);

    useEffect(() => {
        const currentInsights = new Map(allInsightsToDisplay.map(insight => [insight.id, insight]));
        const previousInsights = previousInsightsRef.current;

        const newInsightIds = new Set();
        currentInsights.forEach((insight, id) => {
            if (!previousInsights.has(id)) {
                newInsightIds.add(id);
            }
        });

        if (newInsightIds.size > 0) {
            setEnteringItems(newInsightIds);

            const timer = setTimeout(() => {
                setEnteringItems(new Set());
            }, TIMING_CONFIG.entranceAnimationDuration);

            previousInsightsRef.current = currentInsights;
            return () => clearTimeout(timer);
        }

        previousInsightsRef.current = currentInsights;
    }, [allInsightsToDisplay]);

    const handleRemoveInsight = (insightId: string) => {
        setExitingItems(prev => new Set(prev).add(insightId));

        setTimeout(() => {
            removeInsight(insightId);
            setExitingItems(prev => {
                const newSet = new Set(prev);
                newSet.delete(insightId);
                return newSet;
            });
        }, TIMING_CONFIG.exitAnimationDuration);
    };

    return (
        <div ref={ref} className="insights-vertical-layout-container">
            {allInsightsToDisplay.length > 0
            ? (
                <ul className="insights-vertical-list">
                {allInsightsToDisplay.map((insight, index) => {
                    const isFirstInsight = index === 0;
                    const insightId = insight.id;

                    const isEntering = enteringItems.has(insightId);
                    const isExiting = exitingItems.has(insightId);

                    return (
                        <li
                            key={insightId}
                            className={`
                                insights-vertical-layout-item
                                ${isFirstInsight ? 'insights-vertical-layout-item--active' : 'insights-vertical-layout-item--queue'}
                                ${isEntering ? 'insights-vertical-layout-item--entering' : ''}
                                ${isExiting ? 'insights-vertical-layout-item--exiting' : ''}
                            `}
                        >
                            <button
                                className="insights-vertical-layout-item__close-button"
                                onClick={() => handleRemoveInsight(insightId)}
                                disabled={isExiting}
                            >
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

            {isSmartCaptureEnabled && <LpmamaRow onTooltipHeightChange={onLpmamaTooltipHeightChange} />}
        </div>
    );
});

InsightsVerticalLayout.displayName = 'InsightsVerticalLayout';

export default InsightsVerticalLayout;
