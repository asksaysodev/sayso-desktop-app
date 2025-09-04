// client/src/coachWindow/CoachWindowMain.jsx
import { useEffect, useRef, useState } from 'react';
import { useCoachWindowContext } from '../../context/CoachWindowContext'; 

export default function CoachWindowMain() {

    //REFS
    const containerRef = useRef(null);

    //STATE
    const [isExpanded, setIsExpanded] = useState(false);
    const [selectedProspect, setSelectedProspect] = useState(null);

    //CONTEXT
    const { prospects, startCoach, setProspectId, stopCoach, isCoachActive } = useCoachWindowContext();

    useEffect(() => {
        const updateWindowHeight = () => {
            if (containerRef.current && window.electronAPI) {
                const contentHeight = containerRef.current.scrollHeight;
                // Add some padding to the height
                const windowHeight = Math.max(80, contentHeight);
                window.electronAPI.resizeWindow(windowHeight);
            }
        };

        // Update height when content changes
        updateWindowHeight();
        
        // Use ResizeObserver for automatic height updates
        const resizeObserver = new ResizeObserver(updateWindowHeight);
        if (containerRef.current) {
            resizeObserver.observe(containerRef.current);
        }

        return () => resizeObserver.disconnect();
    }, [prospects]);

    useEffect(() => {
        if (selectedProspect) {
            console.log('selectedProspect', selectedProspect)
            setProspectId(selectedProspect)
        }
    }, [selectedProspect])

    const handleCoach = () => {
        console.log('handleCoach called')
        console.log('isCoachActive', isCoachActive)
        if(isCoachActive) {
            stopCoach()
        } else {
            startCoach()
        }
    }

    return (
        <div className="coach-window" ref={containerRef}>
            <div className={`main-container ${isExpanded ? 'expanded' : ''}`}>
                <select style={{backgroundColor: 'white', color: 'black', borderRadius: '500px', border: 'none', padding: '10px 20px'}} onChange={(e) => setSelectedProspect(e.target.value)}>
                    {prospects.map((prospect) => (
                        <option key={prospect.id} value={prospect.id}>{prospect.name}</option>
                    ))} 
                </select>
                <button onClick={() => setIsExpanded(!isExpanded)} style={{backgroundColor: 'white'}}>
                    {isExpanded ? 'Collapse' : 'Expand'}
                </button>
                <p style={{color: 'white'}}>{isExpanded ? 'Expanded' : 'Collapsed'}</p>
                {
                    selectedProspect && (
                        <button onClick={() => handleCoach()} style={{backgroundColor: 'white'}}>Start Coach</button>
                    )
                }
                
            </div>
        </div>
    );
}
