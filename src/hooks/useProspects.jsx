import apiClient from '../config/axios';

import { useFiles } from './useFiles';

export const useProspects = () => {

  
  const { handleUploadFiles } = useFiles();

  const handleNewProspect = async (prospect, files) => {

    const newProspect = await createProspect(prospect);
    if(files && files.length > 0) {
      await handleUploadFiles(files, 'prospect-files', newProspect.id);
      
    }
    return newProspect;
  };

  const createProspect = async (prospect) => {
    try {
      const response = await apiClient.post('/prospects/create', { prospect });
      return response.data.data;
    } catch (error) {
      console.error('Error creating prospect:', error);
      throw error;
    }

  };

  const getAccountProspects = async () => {
    try {
      const response = await apiClient.get('/prospects/account-prospects');
      return response.data.data;
    } catch (error) {
      console.error('Error getting account prospects:', error);
      throw error;
    }
  };

  const getProspect = async ( prospectId ) => {
    try {
      const response = await apiClient.get(`/prospects/${prospectId}`);
      return response.data.data;
    } catch (error) {
      console.error('Error getting prospect:', error);
      throw error;
    }
  };

  const saveProspectMeeting = async (prospectId, meetingId, meetingTopic, meetingStartTime) => {

    if(!meetingId || !meetingTopic || !meetingStartTime) {
      throw new Error('Meeting ID, topic, and start time are required');
    }

    const data = {
      next_meeting_id: meetingId,
      next_meeting_topic: meetingTopic,
      next_meeting_start_time: meetingStartTime
    }

    try {
      const response = await apiClient.post(`/prospects/save-meeting/${prospectId}`, data);
      return response.data;
    } catch (error) {
      console.error('Error saving prospect meeting:', error);
      throw error;
    }

  }

  const removeZoomMeeting = async (prospectId) => {
    try {
      const response = await apiClient.delete(`/prospects/remove-zoom-meeting/${prospectId}`);
      return response.data;
    } catch (error) {   
      console.error('Error removing zoom meeting:', error);
      throw error;
    }

  } 

  return {
    handleNewProspect,
    createProspect,
    getAccountProspects,
    getProspect,
    saveProspectMeeting,
    removeZoomMeeting
  };
};
    