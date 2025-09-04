import CoachCTA from './CoachCTA';
import '../styles/CTABar.css';

export default function CTABar({active}) {
    return (
        <div className='cta-bar-main'>
            <div className='cta-bar-body'>
                <h2>One click to launch your AI Coach</h2>
                <p>Control your AI assistant with in one click — launch, pause or manage during any meeting.</p>
            </div>
            <CoachCTA active={active} sidebar={false} />
        </div>
    )
}