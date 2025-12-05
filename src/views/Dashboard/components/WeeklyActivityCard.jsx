import { LuChartColumnIncreasing } from "react-icons/lu";
import InformativeCard from "./InformativeCard";

export default function WeeklyActivityCard() {
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

    return (
          <InformativeCard
                icon={<LuChartColumnIncreasing />}
                title={'Activity'}
                description={`${totalMinutes} total minutes this week`}
            >
                <div className='activity-chart-container'>
                    {weekData.map((data, index) => (
                        <div key={index} className='activity-bar-wrapper'>
                            <div className='activity-bar-container'>
                                <div
                                    className='activity-bar'
                                    style={{ height: `${(data.minutes / maxMinutes) * 100}%` }}
                                />
                            </div>
                            <p className='activity-day-label'>{data.day}</p>
                        </div>
                    ))}
                </div>
            </InformativeCard>
    )
}