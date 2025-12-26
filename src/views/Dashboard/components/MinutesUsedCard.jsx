import { LuClock } from "react-icons/lu";
import InformativeCard from "./InformativeCard";
import { useMemo } from "react";

export default function MinutesUsedCard({ accountUsage, isRefetching }) {
    const { usedMinutes = 0, planMinutes = 0 } = accountUsage || {};

    const usedPercentage = useMemo(() => Number(((usedMinutes / planMinutes) * 100).toFixed(2)), [usedMinutes, planMinutes]);
    const safeUsedPercentage = useMemo(() => (typeof usedPercentage === 'number' && !isNaN(usedPercentage)) ? usedPercentage : 0, [usedPercentage]);

    return (
        <InformativeCard
            icon={<LuClock />}
            title={'Minutes Used'}
            description={'Total AI Coach time this billing period'}
            isLoading={isRefetching}
        >
            <div className='card-content-container'>
                <div>
                    <p className='card-content-lighter-text'>
                        <span className='card-content-bold-text'>{usedMinutes}</span> / {planMinutes}min ({safeUsedPercentage}%)
                    </p>
                </div>
                <div className='progress-bar-container'>
                    <div className='progress-bar-fill' style={{ width: `${safeUsedPercentage}%` }}/>
                </div>
            </div>
        </InformativeCard>
    )
}