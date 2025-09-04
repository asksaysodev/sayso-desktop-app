import { useState, useEffect } from 'react';

import { useToast } from '../context/ToastContext';
import { useFiles } from '../hooks/useFiles';

import SaysoModal from './SaysoModal';
import Divider from './Divider';
import FormLine from './FormLine';
import NewFileInput from './NewFileInput';
import ProspectFileCard from './ProspectFileCard';

import { LuX, LuTrash, LuLoader, LuPaperclip } from 'react-icons/lu';


import '../styles/ProspectDetail.css';

export default function ProspectDetail({ prospect, setSelectedProspect, fetchProspects }) {
    
    //STATE
    const [isVisible, setIsVisible] = useState(false);
    const [isClosing, setIsClosing] = useState(false);
    const [backdropVisible, setBackdropVisible] = useState(false);
    const [isDeleting, setIsDeleting] = useState(false);
    const [deleteModalVisible, setDeleteModalVisible] = useState(false);
    const [deleteFileModalVisible, setDeleteFileModalVisible] = useState(false);
    const [prospectFiles, setProspectFiles] = useState({files: []});
    const [existingFiles, setExistingFiles] = useState(null);

    //HOOKS
    const { showToast } = useToast();
    const { handleUploadFiles, fetchFiles } = useFiles();

    //FUNCTIONS
    const handleDeleteProspect = async () => {

        setDeleteModalVisible(false);
        setIsDeleting(true);
        try {
            setTimeout(() => {
                setIsDeleting(false);
                handleClose();
                showToast('success', 'Prospect deleted successfully!');
            }, 1000);
        } catch (error) {
            setIsDeleting(false);
            showToast('error', 'Failed to delete prospect');
            console.error('Error deleting prospect:', error);
            return;
        }
    };

    const handleFetchFiles = async () => {
        const files = await fetchFiles(prospect.id);
        setExistingFiles(files);
    }

    const handleRemoveFile = async (fileId) => {

        setDeleteFileModalVisible(false);
        
        try {
            setTimeout(() => {
                showToast('success', 'File deleted successfully!');
            }, 1000);
        } catch (error) {
            setIsDeleting(false);
            showToast('error', 'Failed to delete file');
            console.error('Error deleting prospect:', error);
            return;
        }
    };

    const handleClose = () => {
        setIsClosing(true);
        setBackdropVisible(false);
        setTimeout(() => {
            setSelectedProspect(null);
        }, 300);
    };

    const handleFileChange = async (e) => {
        const newFiles = Array.from(e.target.files);
        console.log('New files uploaded:', newFiles);
        
        if (newFiles.length === 0) return;
        
        const filesWithStatus = newFiles.map(file => ({
            file: file, // Keep original file object
            uploadStatus: 'loading',
            uploadProgress: 0
        }));
        
        setProspectFiles(prevFiles => ({
            ...prevFiles,
            files: [...prevFiles.files, ...filesWithStatus]
        }));
        
        try {
            const uploadedFiles = await handleUploadFiles(
                newFiles, 
                'prospect', 
                prospect.id 
            );
            
            
            setProspectFiles(prevFiles => ({
                ...prevFiles,
                files: prevFiles.files.map(fileItem => {
                    const isNewFile = newFiles.some(newFile => 
                        newFile.name === fileItem.file?.name && newFile.size === fileItem.file?.size
                    );
                    if (isNewFile) {
                        return {
                            ...fileItem,
                            uploadStatus: 'success',
                            uploadProgress: 100
                        };
                    }
                    return fileItem;
                })
            }));
            
        } catch (error) {
            
            setProspectFiles(prevFiles => ({
                ...prevFiles,
                files: prevFiles.files.map(fileItem => {
                    const isNewFile = newFiles.some(newFile => 
                        newFile.name === fileItem.file?.name && newFile.size === fileItem.file?.size
                    );
                    if (isNewFile) {
                        return {
                            ...fileItem,
                            uploadStatus: 'error',
                            uploadProgress: 0
                        };
                    }
                    return fileItem;
                })
            }));
        }
    };
    
    const removeFile = (indexToRemove) => {
        setProspectFiles(prevFiles => ({
            ...prevFiles,
            files: (prevFiles.files || []).filter((_, idx) => idx !== indexToRemove)
        }));
    };

    //EFFECTS
    useEffect(() => {
        const timer = setTimeout(() => {
            setIsVisible(true);
            setBackdropVisible(true);
        }, 10);
        
        return () => clearTimeout(timer);
    }, []);

    useEffect(() => {
        handleFetchFiles();
    }, [prospect.id]);

    return (
        <>  
            {
                deleteModalVisible && (
                    <SaysoModal 
                        title="Delete Prospect"
                        text={`Are you sure you want to delete this prospect?\nThis action cannot be undone and all related files and meeting data will be permanently removed.`}
                        isDelete = {true}
                        primaryText="Yes, Delete"
                        secondaryText="Cancel"
                        onDeny={() => setDeleteModalVisible(false)}
                        onConfirm={handleDeleteProspect}
                    />
                )
            }
            {
                deleteFileModalVisible && (
                    <SaysoModal 
                        title="Delete File"
                        text={`Are you sure you want to delete this file?\nThis action cannot be undone and the file will be permanently removed.`}
                        isDelete = {true}
                        primaryText="Yes, Delete"
                        secondaryText="Cancel"
                        onDeny={() => setDeleteFileModalVisible(false)}
                        onConfirm={handleRemoveFile}
                    />
                )
            }
            <div 
                className={`prospect-detail-view-container ${backdropVisible ? 'fade-in' : ''} ${isClosing ? 'fade-out' : ''}`} 
                onClick={handleClose}
            />
            <div 
                className={`propect-detail-main-container ${isVisible ? 'slide-in' : ''} ${isClosing ? 'slide-out' : ''}`}
                onClick={(e) => e.stopPropagation()}
            >
                <div className="prospect-detail-header">
                    <button onClick={handleClose} className='prospect-detail-close-button'>
                        <LuX />
                    </button>
                    <div className='prospect-detail-header-content'>
                        <div className='prospect-detail-header-image-container'>
                            <div className='prospect-detail-header-image'>
                                <p>{`${prospect.name.charAt(0).toUpperCase()}${prospect.lastname.charAt(0).toUpperCase()}`}</p>
                            </div>
                        </div>
                        <div className='prospect-detail-header-content-info'>
                            <h2 className='prospect-detail-header-name'>{`${prospect.name} ${prospect.lastname}`}</h2>
                            <p className='prospect-detail-header-email'>{prospect.email}</p>
                            <div className={`prospect-detail-delete-container ${isDeleting ? 'deleting' : ''}`} onClick={() => setDeleteModalVisible(true)}>
                                {
                                    isDeleting ? (
                                        <LuLoader />
                                    ) : (
                                        <LuTrash />
                                    )
                                }
                                <p>Delete Prospect</p>
                            </div>
                        </div>
                    </div>
                    <Divider/>
                </div>
                <div className="propect-detail-body">
                    <div className='prospect-detail-body-information-container'>
                        <form>
                            <FormLine 
                                label='Name' 
                                prospect={prospect} 
                                name='name' 
                                placeholder='John'
                                fetchProspects={fetchProspects}
                            />
                            <FormLine 
                                label='Lastname' 
                                prospect={prospect} 
                                name='lastname' 
                                placeholder='Doe'
                                fetchProspects={fetchProspects}
                            />
                            <FormLine 
                                label='Email' 
                                prospect={prospect} 
                                name='email' 
                                placeholder='john.doe@example.com'
                                fetchProspects={fetchProspects}
                            />
                            <FormLine 
                                label='Company' 
                                prospect={prospect} 
                                name='company' 
                                placeholder='Company Inc.'
                                fetchProspects={fetchProspects}
                            />
                        </form>
                    </div>
                    <Divider/>
                    <div className='prospect-detail-body-files-container'>
                        <div className='prospect-detail-body-files-container-header'>
                            <div className='prospect-detail-body-files-container-header-icon'>
                                <LuPaperclip />
                            </div>
                            <div className='prospect-detail-body-files-container-header-title'>
                                <h4>Related Files:</h4>
                                <p>Upload files that can help you at your next meeting.</p>
                            </div>
                        </div>
                        <NewFileInput 
                            id="prospectFile"
                            onChange={handleFileChange}
                            accept=".pdf,.txt,.docx,.doc"
                        />
                        <div className='prospect-detail-body-files-container-files'>
                            {
                                prospectFiles.files.map((file, index) => (
                                    <ProspectFileCard 
                                        status={file.uploadStatus || 'default'} 
                                        file={file} 
                                        key={index}
                                        progress={file.uploadProgress || 0}
                                        fromDatabase={false}
                                    />
                                ))
                            }
                        </div>
                        <div className='prospect-detail-body-files-container-files'>
                            {
                                existingFiles ? (
                                    <>
                                        {
                                            existingFiles.files.map((file, index) => (
                                                <ProspectFileCard status='default' file={file} key={index} fromDatabase={true}/>
                                            ))
                                        }
                                    </>
                                ):(
                                    <div className='inline-loader'>
                                        <LuLoader />
                                    </div>
                                )
                            }
                        </div>
                    </div>
                </div>
            </div>
        </>
    )
}