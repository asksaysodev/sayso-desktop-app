import './styles.css';
import { ArrowDown, ArrowRight } from 'lucide-react';
import SaysoButton from '@/components/SaysoButton';
import TopDragBar from '@/components/TopDragBar';

interface Props {
    currentVersion: string;
    newVersion: string;
    onUpdate: () => void;
    onDismiss: () => void;
}

export default function Updater({ currentVersion, newVersion, onUpdate, onDismiss }: Props) {
    return (
        <>
            <TopDragBar />
            <div className='updater-container'>
                <div className='updater-content'>
                    <span className='updater-icon-circle'>
                        <ArrowDown size={18} />
                    </span>

                    <p className='updater-title'>New Update Available</p>
                    <p className='updater-description'>A newer version of Sayso is available. If you want to update later, you can find Updates in Settings.</p>

                    <div className='version-pills-container'>
                        <span className='vpc-pill current-version-pill'>v{currentVersion}</span>
                        <span className='vpc-arrow'><ArrowRight size={12} /></span>
                        <span className='vpc-pill newer-version-pill'>v{newVersion}</span>
                    </div>
                </div>

                <div className='updater-footer'>
                    <SaysoButton label={'Update Now'} onClick={onUpdate} fullWidth variant='blue' />
                    <button className='updater-ghost-btn' onClick={onDismiss}>Not Now</button>
                </div>
            </div>
        </>
    )
}
