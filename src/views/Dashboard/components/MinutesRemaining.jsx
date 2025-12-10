import { LuClock } from "react-icons/lu";
import InformativeCard from "./InformativeCard";

export default function MinutesRemaining() {
    return (
        <InformativeCard 
            icon={<LuClock />}
            title={'Minutes Remaining'}
            description={'275 plan min + 40 rollover min'}
        >
            <div className='card-content-container'>
                <div>
                    <p className='card-content-lighter-text'><span className='card-content-bold-text'>315</span> min left</p>
                </div>
            </div>
        </InformativeCard>
    )
}