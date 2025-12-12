import { LuHourglass } from "react-icons/lu";
import InformativeCard from "./InformativeCard";

export default function MinutesRemaining({ accountUsage }) {
    const { remainingMinutes = 0, rolloverMinutes = 0 } = accountUsage || {};

    const cardDescription = `${remainingMinutes} plan min + ${rolloverMinutes} rollover min`;
    const totalMinutesLeft = remainingMinutes + rolloverMinutes;

    return (
        <InformativeCard 
            icon={<LuHourglass />}
            title={'Minutes Remaining'}
            description={cardDescription}
        >
            <div className='card-content-container'>
                <div>
                    <p className='card-content-lighter-text'>
                        <span className='card-content-bold-text'>{totalMinutesLeft}</span> min left
                    </p>
                </div>
            </div>
        </InformativeCard>
    )
}