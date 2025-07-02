import { supabase } from '../config/supabase';
import { useAuth } from '../context/AuthContext';
import { useFiles } from './useFiles';

export const useAccounts = () => {
  const { authToken, globalUser } = useAuth();
  const { getSignedUrl, handleParseFile, uploadFile } = useFiles();

  // Create a new account in the 'accounts' table
  const createAccount = async (formData) => {
    const { company, email, name, lastname } = formData;
    const { data, error } = await supabase
      .from('accounts')
      .insert([
        { company, email, name, lastname }
      ])
      .select();
    if (error) {
      console.error('Error creating account:', error);
      throw error;
    }
    return data?.[0] || null;
  };

  const getAccount = async (email) => {
    const { data, error } = await supabase
      .from('accounts')
      .select('*')
      .eq('email', email);
    if (error) {
      console.error('Error getting account:', error);
      throw error;
    }
    return data?.[0] || null;
  };

  const getAccountFiles = async (accountId) => {
    try {
      const { data, error } = await supabase
        .from('account_files')
        .select('id, file_name, created_at, file_path, file_type, file_size')
        .eq('account_id', accountId)
        .order('created_at', { ascending: false });
      if (error) throw error;
      return data || [];
    } catch (err) {
      console.error('Error fetching account files:', err);
      throw err;
    }
  };

  const addAccountFile = async (accountId, files) => {
    try {
      const uploadedFiles = [];
      // List existing files before upload
      const { data: existingFiles, error: listError } = await supabase.storage
        .from('account-files')
        .list(accountId);
      if (listError) {
        console.error('Error listing existing files:', listError);
      } 
      
      for (const file of files) {
        try {
          const uploadResult = await uploadFile(file, 'account-files', accountId);
          
          // Add file metadata to our array
          uploadedFiles.push({
            account_id: accountId,
            file_name: uploadResult.fileName,
            file_path: uploadResult.filePath,
            file_url: uploadResult.publicUrl,
            file_type: uploadResult.fileType,
            file_size: uploadResult.fileSize,
            created_at: new Date().toISOString()
          });
        } catch (error) {
          console.error(`Error uploading file ${file.name}:`, error);
          continue;
        }
      }

      // If we have files to insert, do it
      if (uploadedFiles.length > 0) {
        const { data, error } = await supabase
          .from('account_files')
          .insert(uploadedFiles)
          .select();
        if (error) {
          console.error('Error creating file records:', error);
          throw error;
        }
        // Generate signed URLs and parse files
        const filesWithUrls = await Promise.all(
          data.map(async (file) => {
            try {
              const signedUrl = await getSignedUrl(file.file_path, 'account-files');
              // Parse the file after getting the signed URL
              const originalFileId = file.id;
              const parseResult = await handleParseFile(signedUrl, originalFileId, globalUser.id);
              return {
                ...file,
                file_url: signedUrl,
                parse_result: parseResult
              };
            } catch (error) {
              console.error(`Error processing file ${file.file_name}:`, error);
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
      console.error('Error in addAccountFile:', error);
      throw error;
    }
  };

  return {
    createAccount,
    getAccount,
    getAccountFiles,
    addAccountFile
  };
};

