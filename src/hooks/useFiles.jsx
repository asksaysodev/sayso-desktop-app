import { supabase } from '../config/supabase';
import { useAuth } from '../context/AuthContext';
import axios from 'axios';

export const useFiles = () => {
  const { authToken, globalUser } = useAuth();

  const getSignedUrl = async (filePath, bucketName) => {
    let attempts = 0;
    const maxAttempts = 3;
    const delay = 1500;
    while (attempts < maxAttempts) {
      try {
        const { data, error } = await supabase.storage
          .from(bucketName)
          .createSignedUrl(filePath, 3600); // URL valid for 1 hour
        if (error) {
          console.error('Error getting signed URL for', filePath, ':', error);
          throw error;
        }
        return data?.signedUrl;
      } catch (error) {
        attempts++;
        if (attempts >= maxAttempts) {
          console.error(`Error in getSignedUrl after ${attempts} attempts:`, error);
          throw error;
        }
        console.warn(`getSignedUrl failed for ${filePath}, retrying in ${delay}ms... (attempt ${attempts + 1}/${maxAttempts})`);
        await new Promise(resolve => setTimeout(resolve, delay));
      }
    }
  };

  const handleParseFile = async (fileSignedUrl, originalFileId, folderId) => {
    console.log('handleParseFile called with:', fileSignedUrl, originalFileId, folderId);
    try {
      const endpoint = `${import.meta.env.VITE_BACKEND_BASE_URL}/files/parse/${folderId}`;
      const response = await axios.post(endpoint, {
        file_url: fileSignedUrl,
        original_file_id: originalFileId
      }, {
        headers: {
          'Content-Type': 'application/json',
          'Authorization': `Bearer ${authToken}` 
        }
      });
      return response.data;
    } catch (error) {
      console.error('Error in handleParseFile:', error);
      throw error;
    }
  };

  const uploadFile = async (file, bucketName, folderPath) => {
    try {
      // Create storage path with timestamp
      const timestamp = Date.now();
      const fileExtension = file.name.split('.').pop();
      const fileNameWithoutExt = file.name.split('.').slice(0, -1).join('.');
      const uniqueFileName = `${fileNameWithoutExt}_${timestamp}.${fileExtension}`;
      const filePath = `${folderPath}/${uniqueFileName}`;

      // Upload to storage
      const { error: uploadError } = await supabase.storage
        .from(bucketName)
        .upload(filePath, file, {
          cacheControl: '3600',
          upsert: false
        });

      if (uploadError) {
        console.error(`Error uploading file ${file.name}:`, uploadError);
        throw uploadError;
      }

      // Get public URL for the file
      const { data: { publicUrl } } = supabase.storage
        .from(bucketName)
        .getPublicUrl(filePath);

      return {
        filePath,
        publicUrl,
        fileName: file.name,
        fileType: file.type,
        fileSize: file.size
      };
    } catch (error) {
      console.error('Error in uploadFile:', error);
      throw error;
    }
  };

  const handleUploadFiles = async (files, type, parentId) => {

    console.log('handleUploadFile called with:', files, type, parentId);

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

      console.log('uploading file:', file);
      try {
        const endpoint = `${import.meta.env.VITE_BACKEND_BASE_URL}/files/upload`;
        
        // Create FormData instance
        const formData = new FormData();
        formData.append('file', file);
        formData.append('type', type);
        formData.append('parent_id', parentId);
  
        console.log('FormData contents:');
        for (let pair of formData.entries()) {
          console.log(pair[0], pair[1]);
        }

        const response = await axios.post(endpoint, formData, {
          headers: {
            'Authorization': `Bearer ${authToken}`,
            'Content-Type': 'multipart/form-data'
          }
        });

        console.log('response:', response.data);

        uploadedFiles.push(response.data);

      } catch (error) {
        console.error('Error in handleUploadFile:', error);
        throw error;
      }
      
    }

  };

  const fetchFiles = async (parentId) => {
    const endpoint = `${import.meta.env.VITE_BACKEND_BASE_URL}/files/fetch/${parentId}`;
    const response = await axios.get(endpoint, {
      headers: {
        'Authorization': `Bearer ${authToken}`,
        'Content-Type': 'application/json',
      }
    });
    return response.data;
  }

  const removeFile = async (fileId) => {
    const endpoint = `${import.meta.env.VITE_BACKEND_BASE_URL}/files/remove/${fileId}`;
    const response = await axios.delete(endpoint, {
      headers: {
        'Authorization': `Bearer ${authToken}`,
        'Content-Type': 'application/json',
      }
    });
    return response.data;
  }

  return {
    getSignedUrl,
    handleParseFile,
    uploadFile,
    handleUploadFiles,
    fetchFiles,
    removeFile
  };
}; 