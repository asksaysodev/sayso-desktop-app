import { useEffect, useRef, useState } from 'react';
import './styles/OnboardingWindow.css';
import Step1TrayIcon from './components/steps/Step1TrayIcon';
import Step2LaunchCoach from './components/steps/Step2LaunchCoach';
import Step3StartCall from './components/steps/Step3StartCall';
import Step4StopSession from './components/steps/Step4StopSession';
import CompletionScreen from './components/CompletionScreen';
import MenuBarMock from './components/animations/MenuBarMock';
import { useOnboardingProgress } from './hooks/useOnboardingProgress';
import updateOnboardingStatus from './services/updateOnboardingStatus';

const STEPS = [
    {
        title: 'Discover your tray menu',
        subtitle: 'The Sayso icon lives in your macOS menu bar. Click it to access your coach anytime.',
        Component: Step1TrayIcon,
    },
    {
        title: 'Launch your coach',
        subtitle: 'Select Launch Coach from the tray menu — or press Ctrl+S from anywhere.',
        Component: Step2LaunchCoach,
    },
    {
        title: 'Start a session',
        subtitle: 'Select a lead type and hit Start. Sayso will be instantly ready to help you during your calls.',
        Component: Step3StartCall,
    },
    {
        title: 'End your session',
        subtitle: 'Hit Stop when your call is done. If you are doing more calls, just click the Reset button to start a session with a fresh context.',
        Component: Step4StopSession,
    },
];

export default function OnboardingWindowApp() {
    const [currentStep, setCurrentStep] = useState(0);
    const [isComplete, setIsComplete] = useState(false);
    const [zoomTransition, setZoomTransition] = useState(false);

    const currentStepRef = useRef(currentStep);
    useEffect(() => { currentStepRef.current = currentStep; }, [currentStep]);

    useEffect(() => {
        const ipc = window.electron?.ipcRenderer;
        if (!ipc) return;
        return ipc.on('onboarding:set-remind-later', () => {
            localStorage.setItem('onboarding_remind_after', String(Date.now() + 24 * 60 * 60 * 1000));
            ipc.send('onboarding:remind-later-ack');
        });
    }, []);

    const advanceFrom = (step: number) => {
        if (step === 0) {
            setZoomTransition(true);
            setTimeout(() => {
                setCurrentStep(1);
                setZoomTransition(false);
            }, 700);
            return;
        }
        if (step < STEPS.length - 1) {
            setCurrentStep(step + 1);
        } else {
            updateOnboardingStatus('complete').catch(() => {});
            setIsComplete(true);
        }
    };

    const { isUnlocked } = useOnboardingProgress((step) => {
        if (currentStepRef.current !== step) return;
        advanceFrom(step);
    });

    const handleNext = () => advanceFrom(currentStep);

    const handleDismiss = () => {
        updateOnboardingStatus('dismissed').catch(() => {});
        window.electron?.ipcRenderer?.send('close-onboarding-window');
    };

    const handleRepeat = () => {
        setIsComplete(false);
        setCurrentStep(0);
    };

    if (isComplete) return <CompletionScreen onRepeat={handleRepeat} />;

    const { title, subtitle, Component } = STEPS[currentStep];

    return (
        <div className="onboarding-window">
            <div className="onboarding-drag-bar">
				<span className="onboarding-drag-bar-text">Welcome to Sayso</span>
			</div>

            <div className={`onboarding-animation-area${currentStep >= 1 ? ' onboarding-animation-area--wallpaper' : ''}`}>
                {currentStep >= 1 && (
                    <MenuBarMock saysoActive className="persistent-menu-bar" />
                )}
                {currentStep === 0 ? (
                    <div className={`macbook-zoom-wrapper${zoomTransition ? ' macbook-zoom-wrapper--zooming' : ''}`}>
                        <Component />
                    </div>
                ) : (
                    <Component />
                )}
            </div>

            <div className="onboarding-text">
                <h1 className="onboarding-title">{title}</h1>
                <p className="onboarding-subtitle">{subtitle}</p>
            </div>

            <div className="onboarding-footer">
                <button className="onboarding-skip" onClick={handleDismiss}>
                    Skip Tour
                </button>
				<div className='onboarding-footer-center-container'>
					<span className="onboarding-counter">{currentStep + 1} / {STEPS.length}</span>
					<div className="onboarding-dots">
					{STEPS.map((_, i) => (
						<button
							key={i}
							className={`onboarding-dot ${i === currentStep ? 'onboarding-dot--active' : i < currentStep ? 'onboarding-dot--done' : ''}`}
							onClick={() => i < currentStep && setCurrentStep(i)}
							disabled={i > currentStep}
						/>
					))}
				</div>
            </div>
                <button
                    className="onboarding-next"
                    onClick={handleNext}
                    disabled={zoomTransition}
                    style={{ visibility: isUnlocked(currentStep) ? 'visible' : 'hidden' }}
                >
                    {currentStep === STEPS.length - 1 ? 'Finish' : 'Next →'}
                </button>
            </div>
        </div>
    );
}
