import { useState, useEffect } from "react";

import FileCard from "../../../components/FileCard";
import NewFileInput from "../../../components/NewFileInput";
import { LuLoader } from "react-icons/lu";

import { useAuth } from "../../../context/AuthContext";
import { useFiles } from "../../../hooks/useFiles";
import { useToast } from "../../../context/ToastContext";

export default function AccountSettingsFilesForm() {

    //STATE
    const [existingFiles, setExistingFiles] = useState([]);
    const [newAccountFiles, setNewAccountFiles] = useState({files: []});
    const [fileToDelete, setFileToDelete] = useState(null);
    const [deleteFileModalVisible, setDeleteFileModalVisible] = useState(false);
    const [deletingFileId, setDeletingFileId] = useState(null);
    const [isLoading, setIsLoading] = useState(false);

    //HOOKS
    const { globalUser } = useAuth();
    const { fetchFiles, handleUploadFiles, removeFile } = useFiles();
    const { showToast } = useToast();

    //FUNCTIONS
    const handleFetchFiles = async () => {

        setIsLoading(true);
        try {
            const files = await fetchFiles(globalUser.id);
            setExistingFiles(files.files);
        } catch (error) {
            console.error('Error fetching files:', error);
        } finally {
            setIsLoading(false);
        }
        
    }

    const handleFileChange = async (e) => {
        const newFiles = Array.from(e.target.files);
        
        if (newFiles.length === 0) return;
        
        const filesWithStatus = newFiles.map(file => ({
            file: file, 
            uploadStatus: 'loading',
            uploadProgress: 0
        }));
        
        setNewAccountFiles(prevFiles => ({
            ...prevFiles,
            files: [...prevFiles.files, ...filesWithStatus]
        }));
        
        try {

            await handleUploadFiles(
                newFiles, 
                'account', 
                globalUser.id 
            );
            
            setNewAccountFiles(prevFiles => ({
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
            
            setNewAccountFiles(prevFiles => ({
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

    const handleDeleteFile = async () => {

        setDeleteFileModalVisible(false);
        const fileId = fileToDelete.id;
        try {
            setDeletingFileId(fileId);
            await removeFile(fileId);
            await handleFetchFiles();
            showToast('success', 'File deleted successfully!');
        } catch (error) {
            showToast('error', 'Failed to delete file');
            console.error('Error deleting file:', error);
            return;
        } finally {
            setDeletingFileId(null);
            setFileToDelete(null);
        }
    };

    const handleCancelDeleteFile = () => {
        setFileToDelete(null);
        setDeleteFileModalVisible(false);
    }

    useEffect(() => {
        if(globalUser?.id) {
            handleFetchFiles();
        }
    }, [globalUser]);


    useEffect(() => {
        setDeleteFileModalVisible(fileToDelete !== null);
    }, [fileToDelete]);

    return (
        <div className='account-settings-files-container'>
            {
                deleteFileModalVisible && (
                    <SaysoModal 
                        title="Delete File"
                        text={`Are you sure you want to delete this file?\nThis action cannot be undone and the file will be permanently removed.`}
                        isDelete = {true}
                        primaryText="Yes, Delete"
                        secondaryText="Cancel"
                        onDeny={handleCancelDeleteFile}
                        onConfirm={handleDeleteFile}
                    />
                )
            }
            <div className='files-upload-button-container'>
                <NewFileInput
                    id="accountFile"
                    onChange={handleFileChange}
                    accept=".pdf,.txt,.docx,.doc"
                />
            </div>
            <div className='files-list-container'>
                {
                    isLoading && (
                        <div className='files-list-container-loading'>
                            <LuLoader className="loading-icon"/>
                        </div>
                    )
                }
                {
                    newAccountFiles && newAccountFiles.files.length > 0 && newAccountFiles.files.map((file, index) => (
                        <FileCard 
                            status={file.uploadStatus || 'default'} 
                            file={file} 
                            key={index}
                            progress={file.uploadProgress || 0}
                            fromDatabase={false}
                            setFileToDelete={setFileToDelete}
                            removeFile={setDeleteFileModalVisible}
                            deletingFileId={deletingFileId}
                        />
                    ))
                }
                {
                    existingFiles && existingFiles.length > 0 && existingFiles.map((file, index) => (
                        <FileCard
                            status={'default'} 
                            file={file} 
                            key={index}
                            fromDatabase={true}
                            setFileToDelete={setFileToDelete}
                            removeFile={setDeleteFileModalVisible}
                            deletingFileId={deletingFileId}
                        />
                    ))
                }
            </div>
        </div>
    )
}