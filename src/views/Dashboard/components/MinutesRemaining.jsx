import { LuHourglass } from "react-icons/lu";
import InformativeCard from "./InformativeCard";
import { useAuth } from "../../../context/AuthContext";

export default function MinutesRemaining({ accountUsage, isRefetching }) {
    const { globalUser } = useAuth();

    const { remainingMinutes = 0, planMinutes = 0 } = accountUsage || {};

    const isTrialing = globalUser?.subscription_status === "trialing";
    const cardDescription = `${planMinutes} ${isTrialing ? 'trial' : 'plan'} minutes`
    const totalMinutesLeft = remainingMinutes;

    return (
        <InformativeCard 
            icon={<LuHourglass />}
            title={'Minutes Remaining'}
            description={cardDescription}
            isLoading={isRefetching}
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