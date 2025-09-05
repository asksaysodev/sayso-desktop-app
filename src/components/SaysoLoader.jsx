import React, { useEffect, useState } from 'react';

import '../styles/SaysoLoader.css';;

export default function SaysoLoader({ textArray = ['Loading...'] }) {

    // const [currentIdx, setCurrentIdx] = useState(0);
    // const [fade, setFade] = useState(true);

    // useEffect(() => {
    //     if (textArray.length <= 1) return;
    //     const interval = setInterval(() => {
    //         setFade(false);
    //         setTimeout(() => {
    //             setCurrentIdx(idx => (idx + 1) % textArray.length);
    //             setFade(true);
    //         }, 300);
    //     }, 3000);
    //     return () => clearInterval(interval);
    // }, [textArray.length]);

    // return (
    //     <div className="sayso-loader" style={{ minHeight: '160px', minWidth: '140px', padding: '24px', display: 'flex', flexDirection: 'column', alignItems: 'center', justifyContent: 'center', width: 'auto' }}>
    //         <img src={'./assets/loader-logo.png'} alt="Sayso Logo" className="sayso-logo-glow" style={{ width: '100px', opacity: '0.9', marginBottom: '16px' }} />
    //         <span className={`sayso-loader-text${fade ? ' fade-in' : ' fade-out'}`}>{textArray[currentIdx]}</span>
    //     </div>
    // )

    return (
        <div className="sayso-loader-container">
            {/* <div className="loadingSpinner"></div> */}
            <div class="loader"></div>
            <p>Loading</p>
        </div>
    )
}