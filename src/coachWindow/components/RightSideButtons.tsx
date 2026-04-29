import { useAppSettingsWindow } from '@/hooks/useAppSettingsWindow';
import { usePlaybookWindow } from '@/hooks/usePlaybookWindow';
import { useCoachWindowStore } from '../../store/coachWindowStore';
import RightSideButton from './RightSideButton';
import { CircleCheckBig, List, Settings, X } from 'lucide-react';

export default function RightSideButtons() {
    const { toggleAppSettingsWindow } = useAppSettingsWindow();
    const { isPlaybookWindowOpen, togglePlaybookWindow } = usePlaybookWindow();
    const isCoachActive = useCoachWindowStore(state => state.isCoachActive);
    const closeCoachWindow = useCoachWindowStore(state => state.closeCoachWindow);
    const coachFeature = useCoachWindowStore(state => state.coachFeature);
    const isCoachLoading = useCoachWindowStore(state => state.isCoachLoading);
    const isResettingCueSession = useCoachWindowStore(state => state.cue.isResettingCueSession);
    const isInsightsLayoutOpen = useCoachWindowStore(state => state.cue.isInsightsLayoutOpen);
    const setIsInsightsLayoutOpen = useCoachWindowStore(state => state.cue_setIsInsightsLayoutOpen);
    const unseenInsightsCount = useCoachWindowStore(state => state.cue.unseenInsightsCount);
    const resetUnseenInsightsCount = useCoachWindowStore(state => state.cue_resetUnseenInsightsCount);
    const isPlaybooksEnabled = useCoachWindowStore(state => state.cue.enabledFeatures.includes('playbooks'));

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
                        <CircleCheckBig size="1em"/>
                    </RightSideButton>
                    {isPlaybooksEnabled && (
                        <RightSideButton onClick={togglePlaybookWindow} disabled={disableButtons} active={isPlaybookWindowOpen}>
                            <List size="1em"/>
                        </RightSideButton>
                    )}
                </>
                : <>
                    <RightSideButton onClick={toggleAppSettingsWindow}>
                        <Settings size="1em"/>
                    </RightSideButton>
                    <RightSideButton onClick={closeCoachWindow}>
                        <X size="1em"/>
                    </RightSideButton>
                </>
            }
        </div>
    );
}
