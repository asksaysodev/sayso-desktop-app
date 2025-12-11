import { useQuery } from "@tanstack/react-query";
import MinutesRemaining from "./MinutesRemaining";
import MinutesUsedCard from "./MinutesUsedCard";
import WeeklyActivityCard from "./WeeklyActivityCard";
import getAccountUsage from "../services/getAccountUsage";

export default function InformativeCardsContainer() {

    const { data: accountUsage, isLoading: isLoadingAccountUsage, error: errorAccountUsage } = useQuery({
        queryKey: ['accountUsage'],
        queryFn: getAccountUsage,
    });

    return (
        <div className='dashboard-cards-container'>
            <MinutesUsedCard accountUsage={accountUsage} />
            <MinutesRemaining accountUsage={accountUsage} />
            <WeeklyActivityCard />
        </div>
    )
}