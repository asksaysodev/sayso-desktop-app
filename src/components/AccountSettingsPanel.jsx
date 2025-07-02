import React, { useState, useRef } from 'react';
// Potentially reuse some styles from Dashboard.css or create a new one
// import '../styles/AccountSettingsPanel.css'; 

const AccountSettingsPanel = ({ onSave, onCancel }) => {
  const [companyName, setCompanyName] = useState('');
  const [companyEmail, setCompanyEmail] = useState('');
  const [files, setFiles] = useState([]); // Store multiple files
  const [isLoading, setIsLoading] = useState(false);
  const fileInputRef = useRef(null);

  const handleFileChange = (e) => {
    // Handle multiple files selected
    setFiles([...e.target.files]); 
  };

  // Trigger hidden file input click
  const triggerFileInput = () => {
    fileInputRef.current?.click();
  };

  const handleSubmit = (e) => {
    e.preventDefault();
    // TODO: Add validation
    if (!companyName || !companyEmail) {
      alert('Please fill in company name and email.');
      return;
    }
    setIsLoading(true);
    console.log('Submitting:', { companyName, companyEmail, files });
    // Call the onSave prop with the data
    onSave({ companyName, companyEmail, files }); 
    setIsLoading(false);
  };

  return (
    <form onSubmit={handleSubmit} className="accountSettingsForm"> {/* Consider a specific class */}
      <h2>Account Settings</h2>
      
      <div className="formGroup">
        <label htmlFor="companyName">Company Name:</label>
        <input 
          type="text" 
          id="companyName" 
          value={companyName}
          onChange={(e) => setCompanyName(e.target.value)} 
          required 
          className="formInput" // Reuse existing class or create new
        />
      </div>

      <div className="formGroup">
        <label htmlFor="companyEmail">Company Email:</label>
        <input 
          type="email" 
          id="companyEmail" 
          value={companyEmail}
          onChange={(e) => setCompanyEmail(e.target.value)} 
          required 
          className="formInput" // Reuse existing class or create new
        />
      </div>

      {
        isLoading ? 
        <Loader message="Uploading files..." /> :
        <>
          <div className="formGroup">
            <label>Context Files:</label>
            {/* Hidden file input */}
            <input 
              type="file" 
              ref={fileInputRef} 
              onChange={handleFileChange} 
              style={{ display: 'none' }} 
              multiple // Allow multiple files
            />
            {/* Button to trigger file input */}
            <button 
              type="button"
              onClick={triggerFileInput} 
              className="btn-secondary" // Example class, adjust as needed
              style={{ marginRight: '10px' }} // Add some spacing
            >
              Choose Files
            </button>
            {/* Display selected file names */}
            {files.length > 0 && (
              <span style={{ marginLeft: '10px', fontSize: '0.9em', color: '#555' }}>
                {files.length} file(s) selected: {files.map(f => f.name).join(', ')}
              </span>
            )}
            {!files.length && (
              <span style={{ marginLeft: '10px', fontSize: '0.9em', color: '#888' }}>
                No files chosen
              </span>
            )}
          </div>

          <div className="formActions"> {/* Reuse existing class or create new */}
              <button type="button" onClick={onCancel} className="btn-cancel">Cancel</button>
              <button type="submit" className="btn-primary">Save Settings</button>
          </div>
        </>
      }
    </form>

  );
};

export default AccountSettingsPanel; 