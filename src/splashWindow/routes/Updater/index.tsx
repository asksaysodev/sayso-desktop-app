import './styles.css';
import { ArrowRight } from 'lucide-react';
import SaysoButton from '@/components/SaysoButton';
import LoginLayout from '@/components/layouts/LoginLayout';

interface Props {
    currentVersion: string;
    newVersion: string;
    onUpdate: () => void;
    onDismiss: () => void;
}

export default function Updater({ currentVersion, newVersion, onUpdate, onDismiss }: Props) {
    return (
		<LoginLayout title={'New Update Available'}>
			<p className='updater-description'>A newer version of Sayso is available. If you want to update later, you can find Updates in Settings.</p>
			<div className='updater-content'>
				<div className='version-pills-container'>
					<span className='vpc-pill current-version-pill'>v{currentVersion}</span>
					<span className='vpc-arrow'><ArrowRight size={12} /></span>
					<span className='vpc-pill newer-version-pill'>v{newVersion}</span>
				</div>
				<div className='updater-footer'>
					<SaysoButton label={'Update Now'} onClick={onUpdate} fullWidth variant='blue' />
					<button className='updater-ghost-btn' onClick={onDismiss}>Not Now</button>
				</div>
			</div>
		</LoginLayout>
    )
}
