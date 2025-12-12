import { useState } from "react";
import { LuChartColumnIncreasing } from "react-icons/lu";
import InformativeCard from "./InformativeCard";
import WeekSelector from "./WeekSelector";
import getWeeklyActivity from "../services/getWeeklyActivity";
import { useQuery } from "@tanstack/react-query";
import WeeklyActivityLoaderSkeleton from "./WeeklyActivityLoaderSkeleton";

export default function WeeklyActivityCard() {
    const [hoveredBar, setHoveredBar] = useState(null);
    const [tooltipY, setTooltipY] = useState(0);
    const [shouldAnimate, setShouldAnimate] = useState(true);
    const [weekOffset, setWeekOffset] = useState(0);

    const { data: weeklyActivity, isLoading: isLoadingWeeklyActivity, error: errorWeeklyActivity } = useQuery({
        queryKey: ['weeklyActivity', weekOffset],
        queryFn: () => getWeeklyActivity(weekOffset),
    });
    
    const { totalMinutes = 0, hasNextWeek, hasPreviousWeek, dailyActivity = [] } = weeklyActivity || {};
        /**
         * dailyActivity es un array de objetos con las siguientes propiedades:
         * {
         *  date: {date: '2025-12-08', dayLong: 'Monday', dayShort: 'M'},
         *  activity: {minutes: 0, sessions: 0, features: Array(0), bookedAppointments: 0}
         * }
         */

    /**
     * Handles week navigation by updating the offset
     * @param {'prev' | 'next'} direction - Direction to navigate
     */
    const handleWeekChange = (direction) => {
        setShouldAnimate(false);
        
        if (direction === 'prev') {
            setWeekOffset(prev => prev + 1);
        } else if (direction === 'next') {
            setWeekOffset(prev => Math.max(0, prev - 1));
        }
        
        setTimeout(() => setShouldAnimate(true), 10);
    };

    const handleMouseMove = (e, index) => {
        const barRect = e.currentTarget.getBoundingClientRect();
        const wrapperRect = e.currentTarget.closest('.activity-bar-wrapper').getBoundingClientRect();
        
        const y = e.clientY - wrapperRect.top;
        setTooltipY(y);
        setHoveredBar(index);
    };

    const handleMouseLeave = () => {
        setHoveredBar(null);
    };

    const renderContent = () => {
        if (isLoadingWeeklyActivity) {
            return <WeeklyActivityLoaderSkeleton />
        }

        return (
            <div className='activity-chart-container'>
                {dailyActivity.map(({ date, activity }, index) => (
                    <div key={date.date} className='activity-bar-wrapper'>
                        {hoveredBar === index && (
                            <div
                                className='activity-tooltip'
                                style={{ top: `${tooltipY}px` }}
                            >
                                {activity?.minutes ?? 0} min
                            </div>
                        )}
                        <div className='activity-bar-container'>
                            <div
                                className={`activity-bar ${shouldAnimate ? 'animate' : ''} ${!activity?.minutes ? 'zero-activity-bar' : ''}`}
                                style={{ height: `${((activity?.minutes ?? 0) / totalMinutes) * 100}%` }}
                                onMouseMove={(e) => handleMouseMove(e, index)}
                                onMouseLeave={handleMouseLeave}
                            />
                        </div>
                        <p className='activity-day-label'>{date.dayShort}</p>
                    </div>
                ))}
            </div>
        );
    };

    return (
          <InformativeCard
                icon={<LuChartColumnIncreasing />}
                title={'Activity'}
                description={`${totalMinutes} total minutes this week`}
                rightContent={<WeekSelector onWeekChange={handleWeekChange} hasNextWeek={hasNextWeek} hasPreviousWeek={hasPreviousWeek} weekOffset={weekOffset}/>}
            >
                {renderContent()}
            </InformativeCard>
    )
}

