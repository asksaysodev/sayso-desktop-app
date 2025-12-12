import { LuClock } from "react-icons/lu";
import InformativeCard from "./InformativeCard";

export default function MinutesUsedCard({ accountUsage }) {
    const { usedMinutes = 0, planMinutes = 0 } = accountUsage || {};

    const usedPercentage = ((usedMinutes / planMinutes) * 100).toFixed(2);

    return (
        <InformativeCard
            icon={<LuClock />}
            title={'Minutes Used'}
            description={'Total AI Coach time this billing period'}
        >
            <div className='card-content-container'>
                <div>
                    <p className='card-content-lighter-text'>
                        <span className='card-content-bold-text'>{usedMinutes}</span> / {planMinutes}min ({usedPercentage}%)
                    </p>
                </div>
                <div className='progress-bar-container'>
                    <div className='progress-bar-fill' style={{ width: `${usedPercentage ?? 0}%` }}/>
                </div>
            </div>
        </InformativeCard>
    )
}