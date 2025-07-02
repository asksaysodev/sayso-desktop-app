import axios from 'axios';

import { supabase } from '../config/supabase';
import { useFiles } from './useFiles';
import { useAuth } from '../context/AuthContext';

export const useProspects = () => {

  
  const { authToken } = useAuth();
  const { getSignedUrl, handleParseFile, uploadFile } = useFiles();

  const handleNewProspect = async (prospect, files) => {
    const newProspect = await createProspect(prospect);
    if(files && files.length > 0) {
      await addProspectFiles(newProspect.id, files);
    }
    return newProspect;
  };

  const createProspect = async (prospect) => {
    console.log("createProspect called with:", prospect);

    const { data, error } = await supabase
      .from('prospects')
      .insert([prospect])
      .select();

    if (error) {
      console.error('Error creating prospect:', error);
      throw error;
    }
    console.log("createProspect result data:", data);
    return data?.[0] || null;
  };

  const addProspectFiles = async (prospectId, files) => {
    console.log('addProspectFiles called with:', prospectId, files);
    try {
      const uploadedFiles = [];

      for (const file of files) {
        try {
          const uploadResult = await uploadFile(file, 'prospect-files', prospectId);
          // Add file metadata to our array
          uploadedFiles.push({
            prospect_id: prospectId,
            file_name: uploadResult.fileName,
            file_path: uploadResult.filePath,
            file_url: uploadResult.publicUrl,
            file_type: uploadResult.fileType,
            file_size: uploadResult.fileSize,
            created_at: new Date().toISOString()
          });
        } catch (error) {
          console.error(`[addProspectFiles] Error uploading file ${file.name}:`, error);
          continue;
        }
      }

      // If we have files to insert, do it
      if (uploadedFiles.length > 0) {
        const { data, error } = await supabase
          .from('prospect_files')
          .insert(uploadedFiles)
          .select();

        if (error) {
          console.error('[addProspectFiles] Error creating file records:', error);
          throw error;
        }

        // Generate signed URLs and parse files
        const filesWithUrls = await Promise.all(
          data.map(async (file) => {
            try {
              const signedUrl = await getSignedUrl(file.file_path, 'prospect-files');
              // Parse the file after getting the signed URL
              const originalFileId = file.id;
              const parseResult = await handleParseFile(signedUrl, originalFileId, prospectId);
              
              return {
                ...file,
                file_url: signedUrl,
                parse_result: parseResult
              };
            } catch (error) {
              console.error(`[addProspectFiles] Error processing file ${file.file_name}:`, error);
              return {
                ...file,
                file_url: null,
                parse_result: null,
                error: error.message
              };
            }
          })
        );
        return filesWithUrls;
      }

      return [];
    } catch (error) {
      console.error('[addProspectFiles] Error in addProspectFiles:', error);
      throw error;
    }
  };

  const getAccountProspects = async (accountId) => {
    const { data, error } = await supabase.from('prospects').select('*').eq('account_id', accountId);
    return data || [];
  };

  const getProspect = async (prospectId) => {
    const { data, error } = await supabase.from('prospects').select('*').eq('id', prospectId);
    return data?.[0] || null;
  };

  const saveProspectMeeting = async (prospectId, meetingId, meetingTopic, meetingStartTime) => {

    const { data, error } = await supabase.from('prospects').update({
      next_meeting_id: meetingId,
      next_meeting_topic: meetingTopic,
      next_meeting_start_time: meetingStartTime
    }).eq('id', prospectId);  
    if(error) {
      console.error('Error saving prospect meeting:', error);
      throw error;
    }
    return data;
  }

  const removeZoomMeeting = async (prospectId) => {
    try {
      const endpoint = `${import.meta.env.VITE_BACKEND_BASE_URL}/prospects/remove-zoom-meeting/${prospectId}`;
      const response = await axios.delete(endpoint, {
        headers: {
          'Authorization': `Bearer ${authToken}`,
          'Content-Type': 'application/json',
        }
      });
      return response.data;
    } catch (error) {   
      console.error('Error removing zoom meeting:', error);
      throw error;
    }

  } 

  return {
    handleNewProspect,
    createProspect,
    addProspectFiles,
    getAccountProspects,
    getProspect,
    saveProspectMeeting,
    removeZoomMeeting
  };
};
    