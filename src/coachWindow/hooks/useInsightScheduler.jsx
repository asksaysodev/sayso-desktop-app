import { useState, useRef, useEffect, useCallback } from "react";

export default function  useInsightScheduler ( {displayMs = 15000, minIntervalMs = 5000 } = {}) {

    //STATE
    const [currentInsight, setCurrentInsight] = useState(null);

    //REFS
    const queueRef = useRef([]);
    const lastStartRef = useRef(0);
    const busyRef = useRef(false);
    const showTimer = useRef(null);
    const hideTimer = useRef(null);

    //FUNCTIONS
    const clearTimers = () => {
        console.log(' [useInsightScheduler] Clearing all timers');
        if(showTimer.current) {
            clearTimeout(showTimer.current);
            showTimer.current = null;
        }
        if(hideTimer.current) {
            clearTimeout(hideTimer.current);
            hideTimer.current = null;
        }
    }

    const step = useCallback(() => {
        console.log('🔄 [useInsightScheduler] step() called', {
            busy: busyRef.current,
            queueLength: queueRef.current.length,
            lastStart: lastStartRef.current,
            minIntervalMs,
            displayMs
        });

        if (busyRef.current) {
            console.log('⏸️ [useInsightScheduler] Busy, skipping step');
            return;
        }
        if (queueRef.current.length === 0) {
            console.log('📭 [useInsightScheduler] Queue empty, skipping step');
            return;
        }

        busyRef.current = true;
        console.log('🚀 [useInsightScheduler] Starting insight display process');

        const now = Date.now();
        
        // Check if the next item in queue is an ice breaker
        const nextItem = queueRef.current[0];
        const isIceBreaker = nextItem && nextItem.isIceBreaker === true;
        
        let wait = 0;
        
        if (isIceBreaker) {
            // Ice breakers display immediately, no waiting
            wait = 0;
            console.log('🧊 [useInsightScheduler] Ice breaker detected - showing immediately');
        } else if (lastStartRef.current === 0) {
            // First non-ice-breaker insight shows immediately
            wait = 0;
            console.log('🎯 [useInsightScheduler] First insight - showing immediately');
        } else {
            // Subsequent insights follow normal timing
            const earliestNextStart = lastStartRef.current + minIntervalMs;
            wait = Math.max(0, earliestNextStart - now);
            console.log('⏰ [useInsightScheduler] Subsequent insight - waiting', wait, 'ms');
        }

        console.log('⏰ [useInsightScheduler] Timing calculation', {
            now,
            earliestNextStart: lastStartRef.current === 0 ? 'N/A (first insight)' : lastStartRef.current + minIntervalMs,
            wait,
            lastStart: lastStartRef.current,
            minIntervalMs,
            isFirstInsight: lastStartRef.current === 0,
            isIceBreaker
        });

        showTimer.current = setTimeout(() => {
            const next = queueRef.current.shift();
            console.log('🎯 [useInsightScheduler] Displaying insight:', next);
            setCurrentInsight(next);
            lastStartRef.current = Date.now();

            hideTimer.current = setTimeout(() => {
                console.log('👋 [useInsightScheduler] Hiding insight after', displayMs, 'ms');
                setCurrentInsight(null);
                busyRef.current = false;
                console.log('✅ [useInsightScheduler] Insight hidden, calling step() again');
                step();
            }, displayMs);
        }, wait);
    }, [displayMs, minIntervalMs]);

    const enqueue = useCallback((insight) => {
        console.log('📥 [useInsightScheduler] Enqueueing insight:', insight);
        queueRef.current.push(insight);
        console.log('📊 [useInsightScheduler] Queue status after enqueue:', {
            queueLength: queueRef.current.length,
            queue: queueRef.current.map(i => i.message || 'No message')
        });
        step();
    }, [step]);

    const reset = useCallback(() => {
        console.log('🔄 [useInsightScheduler] Resetting scheduler');
        clearTimers();
        queueRef.current = [];
        setCurrentInsight(null); // ✅ FIX: Use setCurrentInsight instead of setCurrent
        busyRef.current = false;
        lastStartRef.current = 0;
        console.log('✅ [useInsightScheduler] Scheduler reset complete');
    }, []);

    useEffect(() => {
        console.log('🎬 [useInsightScheduler] Hook initialized with config:', {
            displayMs,
            minIntervalMs
        });
        return () => {
            console.log('🧹 [useInsightScheduler] Hook cleanup - clearing timers');
            clearTimers();
        };
    }, []);

    return { 
        currentInsight, 
        enqueue, 
        reset, 
        pending: queueRef.current.length 
    };
}