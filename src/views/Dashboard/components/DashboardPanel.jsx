import CTABar from '../../../components/CTABar';
import MinutesUsedCard from './MinutesUsedCard';
import MinutesRemaining from './MinutesRemaining';
import WeeklyActivityCard from './WeeklyActivityCard';

export default function DashboardPanel() {
    return (
        <div className='dashboard-panel-main'>
            <div className='dashboard-container-header'>
                <h1>My Dashboard</h1>
            </div>

            <CTABar active={false} />

            <div className='dashboard-cards-container'>
                <MinutesUsedCard />
                <MinutesRemaining />
                <WeeklyActivityCard />
            </div>
        </div>
    )
}