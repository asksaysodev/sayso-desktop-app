import { useState, useEffect } from 'react';

import { LuTrash } from 'react-icons/lu';
import { FaRegCircleCheck, FaRotateRight} from 'react-icons/fa6';
import { GoFile } from 'react-icons/go';

import '../styles/ProspectFileCard.css';

export default function ProspectFileCard({status, file, showDeleteModal}) {

    //STATE
    const [progress, setProgress] = useState(0);
    const [fileStatus, setFileStatus] = useState(status);

    //FUNCTIONS
    const handleRemoveFileClick = async () => {
        setFileStatus('loading');
        showDeleteModal(true);
    }

    //EFFECTS
    useEffect(() => {
        if(status === 'success') {
            setProgress(100);
            return;
        } else if(status === 'loading') {
            const timer = setTimeout(() => {
                if(progress < 90) {
                    setProgress(progress + 10);
                }
            }, 1000);
            return () => clearTimeout(timer);
        }
    }, [progress, status]);

    return (
        <div className={`prospect-file-card-container ${status}`}>
            <div className='prospect-file-card-icon-container'>
                <GoFile />
            </div>
            <div className='prospect-file-card-container-body'>
                <h4>{`${status === 'error' ? 'Upload failed, please try again' : 'File name here.pdf'}`}</h4>
                <p>{`${status === 'error' ? 'File name here.pdf' : '200MB'}`}</p>
                {
                    status === 'error' && (
                        <div className='prospect-file-card-error-container'>
                            <FaRotateRight />
                            <p>Try again</p>
                        </div>
                    )
                }
                {
                    (status === 'loading' || status === 'success') && (
                        <div className='prospect-file-loader-container'>
                        {/* <> */}
                            <div className='prospect-file-loader-progress-bar'>
                                <span style={{ width: `${progress}%` }}></span>
                            </div>
                            <p>{progress}%</p>
                        {/* </> */}
                        </div>     
                    )
                }
            </div>
            <div className='prospect-file-card-status-icon-container'>
                {status === 'success' && <FaRegCircleCheck />}
                {(status === 'error' || status === 'default') && <LuTrash onClick={handleRemoveFileClick} />}
            </div>
        </div>
    )
}