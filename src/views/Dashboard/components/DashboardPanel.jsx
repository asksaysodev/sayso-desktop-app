import CTABar from '../../../components/CTABar';
import InsightsContainer from './InsightsContainer';
import InformativeCardsContainer from './InformativeCardsContainer';

export default function DashboardPanel() {

    return (
        <div className='dashboard-panel-main'>
            <div className='dashboard-container-header'>
                <h1>My Dashboard</h1>
            </div>

            <CTABar active={false} />

            <InformativeCardsContainer />

            <InsightsContainer />
        </div>
    )
}