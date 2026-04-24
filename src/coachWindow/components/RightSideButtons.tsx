import { useAppSettingsWindow } from '@/hooks/useAppSettingsWindow';
import { useCoachWindowStore } from '../../store/coachWindowStore';
import RightSideButton from './RightSideButton';
import { CircleCheckBig, List, Settings, X } from 'lucide-react';
import { LuEye, LuEyeClosed } from 'react-icons/lu';

const ICON_SIZE = 16;

export default function RightSideButtons() {
    const { toggleAppSettingsWindow } = useAppSettingsWindow();
    const isCoachActive = useCoachWindowStore(state => state.isCoachActive);
    const closeCoachWindow = useCoachWindowStore(state => state.closeCoachWindow);
    const coachFeature = useCoachWindowStore(state => state.coachFeature);
    const isCoachLoading = useCoachWindowStore(state => state.isCoachLoading);
    const isResettingCueSession = useCoachWindowStore(state => state.cue.isResettingCueSession);
    const isInsightsLayoutOpen = useCoachWindowStore(state => state.cue.isInsightsLayoutOpen);
    const setIsInsightsLayoutOpen = useCoachWindowStore(state => state.cue_setIsInsightsLayoutOpen);
    const unseenInsightsCount = useCoachWindowStore(state => state.cue.unseenInsightsCount);
    const resetUnseenInsightsCount = useCoachWindowStore(state => state.cue_resetUnseenInsightsCount);

    const disableButtons = isCoachLoading || isResettingCueSession;

    const handleOpenInsightsLayout = () => {
        if (!isInsightsLayoutOpen) {
            resetUnseenInsightsCount();
        }
        setIsInsightsLayoutOpen(!isInsightsLayoutOpen);
    }

    return (
        <div className='right-side-buttons-container'>
            {isCoachActive
                ? <>
                    <RightSideButton onClick={handleOpenInsightsLayout} disabled={disableButtons} active={isInsightsLayoutOpen}>
                        {unseenInsightsCount > 0 && <span className="cue-eye-toggle-notification-badge"></span>}
                        <CircleCheckBig size={ICON_SIZE}/>
                    </RightSideButton>
                    <RightSideButton onClick={() => { }} disabled> 
                        {/*disabled untill we develop this feature*/}
                        <List size={ICON_SIZE}/>
                    </RightSideButton>
                </>
                : <>
                    <RightSideButton onClick={toggleAppSettingsWindow}>
                        <Settings size={ICON_SIZE}/>
                    </RightSideButton>
                    <RightSideButton onClick={closeCoachWindow}>
                        <X size={ICON_SIZE}/>
                    </RightSideButton>
                </>
            }
        </div>
    );
}
