import { useState, useEffect, useRef, useMemo, forwardRef } from 'react';
import { useCoachWindowStore, CUE_CONFIG } from '../../store/coachWindowStore';
import { LuX, LuPin, LuPinOff } from 'react-icons/lu';
import LpmamaRow from './LpmamaRow';
import '../styles/InsightVerticalLayout.css';

const TIMING_CONFIG = {
    entranceAnimationDuration: 400,
    exitAnimationDuration: 250,
};

const MAX_UNPINNED = 5;

interface Props {
    onLpmamaTooltipHeightChange: (height: number) => void;
    maxListHeight?: number;
}

const InsightsVerticalLayout = forwardRef<HTMLDivElement, Props>(({ onLpmamaTooltipHeightChange, maxListHeight }, ref) => {
    const insightsQueue = useCoachWindowStore(state => state.cue.insightsQueue);
    const removeInsight = useCoachWindowStore(state => state.cue_removeInsight);
    const togglePinInsight = useCoachWindowStore(state => state.cue_togglePinInsight);
    const isSmartCaptureEnabled = useCoachWindowStore(state => state.cue.enabledFeatures.includes('smart_capture'));

    const [enteringItems, setEnteringItems] = useState(new Set());
    const [exitingItems, setExitingItems] = useState(new Set());
    const [hoveredPinId, setHoveredPinId] = useState<string | null>(null);
    const previousInsightsRef = useRef(new Map());

    const pinnedInsights = useMemo(() =>
        insightsQueue
            .filter(i => i.pinned)
            .sort((a, b) => (b.createdAt ?? 0) - (a.createdAt ?? 0))
            .slice(0, CUE_CONFIG.maxPinnedInsights),
        [insightsQueue]
    );

    const unpinnedInsights = useMemo(() =>
        insightsQueue
            .filter(i => !i.pinned)
            .slice(0, MAX_UNPINNED),
        [insightsQueue]
    );

    const allInsightsToDisplay = useMemo(() =>
        [...pinnedInsights, ...unpinnedInsights],
        [pinnedInsights, unpinnedInsights]
    );

    const isPinLimitReached = pinnedInsights.length >= CUE_CONFIG.maxPinnedInsights;

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

    const renderInsight = (insight: typeof insightsQueue[0], index: number, section: 'pinned' | 'unpinned') => {
        const isFirstUnpinned = section === 'unpinned' && index === 0;
        const insightId = insight.id;
        const isEntering = enteringItems.has(insightId);
        const isExiting = exitingItems.has(insightId);
        const isPinned = !!insight.pinned;
        const isPinHovered = hoveredPinId === insightId;
        const isPinDisabled = !isPinned && isPinLimitReached;

        const stateClass = isPinned
            ? 'insights-vertical-layout-item--pinned'
            : isFirstUnpinned
                ? 'insights-vertical-layout-item--active'
                : 'insights-vertical-layout-item--queue';

        return (
            <li
                key={insightId}
                className={`
                    insights-vertical-layout-item
                    ${stateClass}
                    ${isEntering ? 'insights-vertical-layout-item--entering' : ''}
                    ${isExiting ? 'insights-vertical-layout-item--exiting' : ''}
                `}
            >
                <button
                    className={`insights-vertical-layout-item__pin-button ${isPinned ? 'insights-vertical-layout-item__pin-button--active' : ''}`}
                    onClick={() => togglePinInsight(insightId)}
                    onMouseEnter={() => setHoveredPinId(insightId)}
                    onMouseLeave={() => setHoveredPinId(null)}
                    disabled={isPinDisabled || isExiting}
                    title={isPinned ? 'Unpin' : isPinDisabled ? 'Max 3 pins reached' : 'Pin'}
                >
                    {isPinned && isPinHovered ? <LuPinOff size={11}/> : <LuPin size={11}/>}
                </button>
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
    };

    const hasAny = allInsightsToDisplay.length > 0;

    return (
        <div ref={ref} className="insights-vertical-layout-container">
            {hasAny ? (
                <ul
                    className="insights-vertical-list"
                    style={maxListHeight ? { maxHeight: maxListHeight } : undefined}
                >
                    {pinnedInsights.map((insight, index) => renderInsight(insight, index, 'pinned'))}
                    {pinnedInsights.length > 0 && unpinnedInsights.length > 0 && (
                        <li className="insights-vertical-layout-divider" aria-hidden="true" />
                    )}
                    {unpinnedInsights.map((insight, index) => renderInsight(insight, index, 'unpinned'))}
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
