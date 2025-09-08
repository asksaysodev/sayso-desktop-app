
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

  const updateAccount = async (updateData) => {
    try {
      if(!updateData) {
        throw new Error('Update data is required');
      }
      await apiClient.put(`/accounts/update-account`, { updateData });
      return
    } catch (error) {   
      console.error('Error in updateProspect:', error);
      throw error;
    }

  } 

  const getCompanyById = async (companyId) => {
    try {
      const response = await apiClient.get(`/accounts/company/${companyId}`);
      return response.data.data;
    } catch (error) {
      console.error('Error getting company:', error);
      throw error;
    }
  }

  return {
    createAccount,
    getAccount,
    updateAccount,
    getCompanyById
  };
};

