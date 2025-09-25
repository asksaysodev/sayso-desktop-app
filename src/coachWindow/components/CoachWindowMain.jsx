import { useEffect, useRef, useState } from 'react';

import { MdDragIndicator } from 'react-icons/md';
import { IoChevronDown , IoChevronUp, IoClose } from 'react-icons/io5';
import { SlMagnifier } from 'react-icons/sl';

import { useCoachWindowContext } from '../../context/CoachWindowContext'; 

import CoachButtons from './CoachButtons';
import SmartCaptureBox from './SmartCaptureBox';
import CoachInsightContainer from './CoachInsightContainer';


export default function CoachWindowMain() {

    //REFS
    const containerRef = useRef(null);

    //STATE
    const [selectedProspect, setSelectedProspect] = useState(null);
    const [isSmartCaptureActive, setIsSmartCaptureActive] = useState(false);
    const [isSelectProspectModalOpen, setIsSelectProspectModalOpen] = useState(false);
    const [inputSearch, setInputSearch] = useState('');
    const [displayedProspects, setDisplayedProspects] = useState([]);
    // const [displayInsight, setDisplayInsight] = useState(null);
    // const [isClosingInsight, setIsClosingInsight] = useState(false);
    // const [insightQueue, setInsightQueue] = useState([]);
    // const [lastInsightTime, setLastInsightTime] = useState(0);
    // const closeTimeoutRef = useRef(null);
    // const [isInsightDisplaying, setIsInsightDisplaying] = useState(false);

    //CONTEXT / HOOKS
    const { prospects, 
        startCoach, 
        setProspectId, 
        stopCoach, 
        isCoachActive, 
        callDurationInSeconds, 
        isCoachLoading ,
        currentInsight,
        signals
    } = useCoachWindowContext();

    //FUNCTIONS
    const handleSmartCapture = () => {
        setIsSmartCaptureActive(!isSmartCaptureActive);
    }

    const handleProspectSelectClick = () => {
        setIsSelectProspectModalOpen(!isSelectProspectModalOpen);
        setInputSearch('');
    }

    const handleProspectSelect = (prospect) => {
        setSelectedProspect(prospect);
        setIsSelectProspectModalOpen(false);
        setInputSearch('');
    }

    const handleCoach = async () => {
        console.log('handleCoach called')
        console.log('isCoachActive', isCoachActive)
        if(isCoachActive) {
            await stopCoach()
        } else {
            if(!selectedProspect) {
                console.log('No prospect selected')
                return
            }
            await startCoach(selectedProspect.id)
        }
    }

    const handleCloseCoachWindow = () => {
        console.log('🎯 [CoachWindowMain] Close button clicked, closing window directly');
        if(window.electronAPI) {
            window.electronAPI.closeCoachWindow()
        }
    }

    useEffect(() => {
        let isResizing = false; // Flag to prevent feedback loop
        
        const updateWindowSize = () => {
            if (containerRef.current && window.electronAPI && !isResizing) {
                isResizing = true; // Set flag to prevent recursive calls
                
                const contentHeight = containerRef.current.scrollHeight;
                const contentWidth = containerRef.current.scrollWidth;
                
                // Calculate responsive dimensions
                const windowHeight = Math.max(48, (isSelectProspectModalOpen ? 298 : contentHeight + 8 ))
                
                // Width based on content with min/max constraints
                const minWidth = 300;  // Minimum usable width
                const maxWidth = 1200; // Maximum width before it gets too wide
                const padding = 20;    // Padding around content
                
                // Add 700px to width when displayInsight is true
                const insightWidth = currentInsight ? 700 : 0;
                const windowWidth = Math.max(minWidth, Math.min(maxWidth, contentWidth + padding + insightWidth));
                
                window.electronAPI.resizeWindow(windowWidth, windowHeight);
                
                // Reset flag after a short delay
                setTimeout(() => {
                    isResizing = false;
                }, 100);
            }
        };

        // Update size when content changes
        updateWindowSize();
        
        // Use ResizeObserver for automatic size updates
        const resizeObserver = new ResizeObserver(updateWindowSize);
        if (containerRef.current) {
            resizeObserver.observe(containerRef.current);
        }

        return () => resizeObserver.disconnect();
    }, [prospects, isSelectProspectModalOpen, currentInsight]);

    useEffect(() => {
        if (selectedProspect) {
            setProspectId(selectedProspect.id)
        }
    }, [selectedProspect])

    useEffect(() => {
        if(inputSearch === '') {
            setDisplayedProspects(prospects);
            return;
        }

        const filteredProspects = prospects.filter(prospect => prospect.name.toLowerCase().includes(inputSearch.toLowerCase()) || prospect.email.toLowerCase().includes(inputSearch.toLowerCase()) || prospect.company.toLowerCase().includes(inputSearch.toLowerCase()));

        if(filteredProspects.length > 0) {
            setDisplayedProspects(filteredProspects);
        } else {
            setDisplayedProspects([]);
        }

    }, [prospects, inputSearch]);


    return (
        <div className="coach-window" ref={containerRef}>
            <div className={`main-container coach-box-bubble`}>
                <div className='main-toolbar'>

                    <div className="coach-window-drag-container">
                        <MdDragIndicator/>
                    </div>
                    <div className="coach-window-divider"></div>
                    <div className='coach-smart-capture-toggle-container' onClick={handleSmartCapture}>
                        <p>Show Smart Capture</p>
                        <div className='coach-smart-capture-toggle' data-active={isSmartCaptureActive} >
                            <span></span>
                        </div>
                    </div>
                    <div className="coach-window-divider"></div>
                    {
                        selectedProspect ? (
                            <button className='coach-window-selected-prospect-button' onClick={() => handleProspectSelectClick()} data-open={isSelectProspectModalOpen} disabled={isCoachActive}>
                                <div className='coach-window-select-prospect-button-text'>
                                    <div className='coach-window-selected-prospect-initials-container'>
                                        <p>{selectedProspect.name.charAt(0).toUpperCase()}{selectedProspect.lastname.charAt(0).toUpperCase()}</p>
                                    </div>
                                    <div className='coach-window-selected-prospect-button-text-container'>
                                        <p >{selectedProspect.name} {selectedProspect.lastname}</p>
                                        {
                                            !isCoachActive && (
                                                <>
                                                    {isSelectProspectModalOpen ? <IoChevronUp /> : <IoChevronDown />}
                                                </>

                                            )
                                        }
                                    </div>
                                </div>
                            </button>
                        ):(
                            <button className='coach-window-select-prospect-button' onClick={() => handleProspectSelectClick()} data-open={isSelectProspectModalOpen}>
                                <div className='coach-window-select-prospect-button-text'>
                                    <p>Select Prospect</p>
                                    <>
                                        {isSelectProspectModalOpen ? <IoChevronUp /> : <IoChevronDown />}
                                    </>
                                </div>
                            </button>
                        )
                    }
                    {
                        isSelectProspectModalOpen && (
                            <div className='coach-window-select-prospect-modal' data-open={isSelectProspectModalOpen}>
                                <div className='coach-window-select-prospect-modal-content'>
                                    <div className='searchbar-container'>
                                        <SlMagnifier />
                                        <input type="text" placeholder='Search Prospects' value={inputSearch} onChange={(e) => setInputSearch(e.target.value)} />
                                    </div>
                                    {
                                        inputSearch !== '' && displayedProspects.length === 0 ? (
                                            <p className='no-prospects-found-text'>No prospects found</p>
                                        ):(
                                            <ul>
                                                {displayedProspects.map((prospect, index) => (
                                                    <div key={prospect.id}>
                                                        <li key={prospect.id} onClick={() => handleProspectSelect(prospect)} >
                                                            <div className='prospect-initials-container'>
                                                                <p>{prospect.name.charAt(0).toUpperCase()}{prospect.lastname.charAt(0).toUpperCase()}</p>
                                                            </div>
                                                            <div className='prospect-info-container'>
                                                                <h4>{prospect.name} {prospect.lastname}</h4>
                                                                <p>{prospect.email}</p>
                                                            </div>
                                                        </li>
                                                        {index !== displayedProspects.length - 1 && (
                                                            <span className='ul-divider'></span>
                                                        )}
                                                    </div>
                                                ))}
                                            </ul>
                                        )
                                    }
                                </div>
                            </div>
                            )
                    }
                    {
                        selectedProspect && (
                            <CoachButtons
                                isCoachActive={isCoachActive}
                                handleCoach={handleCoach}
                                callDurationInSeconds={callDurationInSeconds}
                                isCoachLoading={isCoachLoading}
                            />
                        )
                    }
                    {
                        !isCoachActive && (
                            <button className='close-coach-button-container' onClick={() => handleCloseCoachWindow()}>
                                <IoClose />
                            </button>
                        )
                    }
                </div>
                {
                    isSmartCaptureActive && (
                        <SmartCaptureBox signals={signals} />
                    )
                }
            </div>
            {
                currentInsight && isCoachActive && (
                    <CoachInsightContainer displayInsight={currentInsight?.message}  />
                )
            }
        </div>
    );
}
