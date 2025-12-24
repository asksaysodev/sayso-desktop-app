import { LuRefreshCcw } from "react-icons/lu";
import useRefreshDashboardData from "../hooks/useRefreshDashboardData";

export default function RefreshWidgetsDataButton() {
    const { refreshDashboardData } = useRefreshDashboardData();

    return (
        <button onClick={refreshDashboardData}>
            <LuRefreshCcw color="var(--sayso-indigo)" size={20}/>
        </button>
    )
}