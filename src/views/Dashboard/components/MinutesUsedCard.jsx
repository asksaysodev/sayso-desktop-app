import { LuClock } from "react-icons/lu";
import InformativeCard from "./InformativeCard";

export default function MinutesUsedCard() {
    return (
        <InformativeCard
            icon={<LuClock />}
            title={'Minutes Used'}
            description={'Total AI Coach time this billing period'}
        >
            <div className='card-content-container'>
                <div>
                    <p className='card-content-lighter-text'><span className='card-content-bold-text'>185</span> / 500min (37%)</p>
                </div>
                <div className='progress-bar-container'>
                    <div className='progress-bar-fill' style={{ width: '37%' }}/>
                </div>
            </div>
        </InformativeCard>
    )
}