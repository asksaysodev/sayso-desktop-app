import apiClient from '../config/axios';

export const useFiles = () => {


  const handleParseFile = async (fileSignedUrl, originalFileId, folderId) => {
    try {
      const response = await apiClient.post('/files/parse/' + folderId, {
        file_url: fileSignedUrl,
        original_file_id: originalFileId
      });
      return response.data;
    } catch (error) {
      console.error('Error in handleParseFile:', error);
      throw error;
    }
  };

  const uploadFile = async (file, type, parentId) => {
    if(!file) {
      console.error('No file provided');
      return;
    }

    if(!type) {
      console.error('No type provided');
      return;
    }

    if(!parentId) {
      console.error('No parentId provided');
      return;
    }

    try {
      // Create FormData instance
      const formData = new FormData();
      formData.append('file', file);
      formData.append('type', type);
      formData.append('parent_id', parentId);

      const response = await apiClient.post('/files/upload', formData, {
        headers: {
          'Content-Type': 'multipart/form-data'
        }
      });

      return response.data;

    } catch (error) {
      console.error('Error in uploadFile:', error);
      throw error;
    }
  };

  const handleUploadFiles = async (files, type, parentId) => {
    if(!files || files.length === 0) {
      console.error('No files provided');
      return;
    }

    if(!type) {
      console.error('No type provided');
      return;
    }

    if(!parentId) {
      console.error('No parentId provided');
      return;
    }

    const uploadedFiles = [];

    for (const file of files) {
      try {
        // Create FormData instance
        const formData = new FormData();
        formData.append('file', file);
        formData.append('type', type);
        formData.append('parent_id', parentId);

        const response = await apiClient.post('/files/upload', formData, {
          headers: {
            'Content-Type': 'multipart/form-data'
          }
        });

        uploadedFiles.push(response.data);

      } catch (error) {
        console.error('Error in handleUploadFile:', error);
        throw error;
      }
    }

    return uploadedFiles;
  };

  const fetchFiles = async (parentId) => {
    const response = await apiClient.get('/files/fetch/' + parentId);
    return response.data;
  }

  const removeFile = async (fileId) => {
    const response = await apiClient.delete('/files/remove/' + fileId);
    return response.data;
  }

  return {
    handleParseFile,
    uploadFile,
    handleUploadFiles,
    fetchFiles,
    removeFile
  };
}; 