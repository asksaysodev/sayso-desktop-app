import apiClient from '../config/axios';

export const useAdmin = () => {

  const postCueSignals = async ( leadType, signals ) => {
    try {
      if(!leadType || !signals) {
        throw new Error('Lead type and signals are required');
      }
      const response = await apiClient.post('/support/cue/signals', { leadType, signals });
      return response.data;
    } catch (error) {
      console.error('Error creating account:', error);
      throw error;
    }
  };


  return {
    postCueSignals
  };
};

