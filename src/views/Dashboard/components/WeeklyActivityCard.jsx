import { useState } from "react";
import { LuChartColumnIncreasing } from "react-icons/lu";
import InformativeCard from "./InformativeCard";
import WeekSelector from "./WeekSelector";

export default function WeeklyActivityCard() {
    const [hoveredBar, setHoveredBar] = useState(null);
    const [tooltipY, setTooltipY] = useState(0);
    const [currentWeek, setCurrentWeek] = useState(null);
    const [shouldAnimate, setShouldAnimate] = useState(true);

    const weekData = [
        { day: 'M', minutes: 45 },
        { day: 'T', minutes: 32 },
        { day: 'W', minutes: 28 },
        { day: 'T', minutes: 18 },
        { day: 'F', minutes: 52 },
        { day: 'S', minutes: 15 },
        { day: 'S', minutes: 8 }
    ];

    const totalMinutes = weekData.reduce((sum, day) => sum + day.minutes, 0);
    const maxMinutes = Math.max(...weekData.map(d => d.minutes));

    // esto cuando hagamos un fetch de data al cambiar de fecha no sera necesario, usaremos el loading state
    const handleWeekChange = (weekRange) => {
        setShouldAnimate(false);
        setCurrentWeek(weekRange);
        setTimeout(() => setShouldAnimate(true), 10);
    };

    const handleMouseMove = (e, index) => {
        const rect = e.currentTarget.getBoundingClientRect();
        const y = e.clientY - rect.top;
        setTooltipY(y);
        setHoveredBar(index);
    };

    const handleMouseLeave = () => {
        setHoveredBar(null);
    };

    return (
          <InformativeCard
                icon={<LuChartColumnIncreasing />}
                title={'Activity'}
                description={`${totalMinutes} total minutes this week`}
                rightContent={<WeekSelector onWeekChange={handleWeekChange} />}
            >
                <div className='activity-chart-container'>
                    {weekData.map((data, index) => (
                        <div key={index} className='activity-bar-wrapper'>
                            {hoveredBar === index && (
                                <div
                                    className='activity-tooltip'
                                    style={{ top: `${tooltipY}px` }}
                                >
                                    {data.minutes} min
                                </div>
                            )}
                            <div className='activity-bar-container'>
                                <div
                                    className={`activity-bar ${shouldAnimate ? 'animate' : ''}`}
                                    style={{ height: `${(data.minutes / maxMinutes) * 100}%` }}
                                    onMouseMove={(e) => handleMouseMove(e, index)}
                                    onMouseLeave={handleMouseLeave}
                                />
                            </div>
                            <p className='activity-day-label'>{data.day}</p>
                        </div>
                    ))}
                </div>
            </InformativeCard>
    )
}

