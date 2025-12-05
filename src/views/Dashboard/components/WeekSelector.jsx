import { useState } from "react";
import {LuChevronLeft,LuChevronRight} from "react-icons/lu";
import dayjs from "dayjs";
import isoWeek from "dayjs/plugin/isoWeek";

dayjs.extend(isoWeek);

export default function WeekSelector({ onWeekChange }) {
    const [currentWeek, setCurrentWeek] = useState(dayjs());
    
    const weekStart = currentWeek.startOf('isoWeek');
    const weekEnd = currentWeek.endOf('isoWeek');
    
    const dateRange = [
        weekStart.format('YYYY-MM-DD'), 
        weekEnd.format('YYYY-MM-DD')
    ];
    
    const handlePrevWeek = () => {
        const newWeek = currentWeek.subtract(1, 'week');
        setCurrentWeek(newWeek);
        onWeekChange?.(dateRange);
    };

    const handleNextWeek = () => {
        const newWeek = currentWeek.add(1, 'week');
        setCurrentWeek(newWeek);
        onWeekChange?.(dateRange);
    };
    
    const dateRangeText = `${weekStart.format('MMM D')} - ${weekEnd.format('MMM D')}`;    
    
    return (
        <div className="week-selector-container">
            <button className="week-selector-button" onClick={handlePrevWeek}>
                <LuChevronLeft/>
            </button>

            <span className="week-selector-date-text">{dateRangeText}</span>

            <button className="week-selector-button" onClick={handleNextWeek}>
                <LuChevronRight/>
            </button>
        </div>
    )
}