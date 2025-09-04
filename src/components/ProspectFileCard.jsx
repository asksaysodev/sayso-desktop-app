import { useState, useEffect } from 'react';

import { LuTrash } from 'react-icons/lu';
import { FaRegCircleCheck, FaRotateRight} from 'react-icons/fa6';
import { GoFile } from 'react-icons/go';

import '../styles/ProspectFileCard.css';

export default function ProspectFileCard({status, file, showDeleteModal, progress: propProgress = 0, fromDatabase = false, setFormData}) {

    //STATE
    const [progress, setProgress] = useState(propProgress);
    const [fileStatus, setFileStatus] = useState(status);

    //FUNCTIONS
    const formatFileSize = (bytes) => {
        if (bytes === 0) return '0 MB';
        const mb = bytes / (1024 * 1024);
        return `${mb.toFixed(2)} MB`;
    };

    const handleRemoveFileClick = async () => {
        if(fromDatabase) {
            //REMOVE FROM DATABASE
            return;
        } else {
            //REMOVE FROM FORM LOCAL DATA
            setFormData(prevData => ({
                ...prevData,
                files: prevData.files.filter(f => f.name !== file.name)
            }));
        }
    }

    //EFFECTS
    useEffect(() => {
        setProgress(propProgress);
    }, [propProgress]);

    useEffect(() => {
        if(status === 'success') {
            setProgress(100);
            return;
        } else if(status === 'loading' && propProgress === 0) {
            // Only animate progress if no prop progress is provided
            const timer = setTimeout(() => {
                if(progress < 90) {
                    setProgress(progress + 10);
                }
            }, 1000);
            return () => clearTimeout(timer);
        }
    }, [progress, status, propProgress]);

    return (
        <div className={`prospect-file-card-container ${status}`}>
            <div className='prospect-file-card-icon-container'>
                <GoFile />
            </div>
            <div className='prospect-file-card-container-body'>
                <h4>{`${status === 'error' ? 'Upload failed, please try again' : fromDatabase ? file.file_name : file.file?.name || file.name}`}</h4>
                <p>{`${status === 'error' ? file.name : formatFileSize(fromDatabase ? file.file_size : file.file?.size || file.size)}`}</p>
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
                            <div className='prospect-file-loader-progress-bar'>
                                <span style={{ width: `${progress}%` }}></span>
                            </div>
                            <p>{progress}%</p>
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