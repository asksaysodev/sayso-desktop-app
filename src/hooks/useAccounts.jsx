
import apiClient from '../config/axios';

export const useAccounts = () => {

  const createAccount = async ( accountData ) => {
    try {
      const response = await apiClient.post('/accounts/create', { accountData });
      return response.data.data;
    } catch (error) {
      console.error('Error creating account:', error);
      throw error;
    }
  };

  const getAccount = async (email) => {
    try {
      const response = await apiClient.get(`/accounts/${email}`);
      return response.data.data;
    } catch (error) {
      console.error('Error getting account:', error);
      throw error;
    }
  };

  return {
    createAccount,
    getAccount,
  };
};

