import { useState, useEffect, useCallback } from 'react';
import '../styles/Dashboard.css';
import { useSalesCoach } from '../hooks/useSalesCoach';
import { useProspectsContext } from '../context/ProspectsContext';
import { useAuth } from '../context/AuthContext';
import BtnPrimary from '../components/BtnPrimary';
import BtnMain from '../components/BtnMain';
import ContentTemplate from '../components/ContentTemplate';
import FileInput from '../components/FileInput';
import { useProspects } from '../hooks/useProspects';
import { capitalizeWords } from '../context/AppContext';
import FileListContainer from '../components/FileListContainer';
import MeetingPickerModal from '../components/MeetingPickerModal';
import { useTimes } from '../hooks/useTimes';
import { useNavigate } from 'react-router-dom';
import SaysoLoader from '../components/SaysoLoader';
import ConnectZoomButton from '../components/ConnectZoomButton';
import { useFiles } from '../hooks/useFiles';
import { useSalesCoachContext } from '../context/SalesCoachContext';

// --- Add Prospect Form Component ---
const AddProspectForm = ({ formData, onFormChange }) => {
  const handleFileChange = (e) => {
    const newFiles = Array.from(e.target.files);
    onFormChange({
      ...formData,
      files: [...formData.files, ...newFiles].filter((file, index, self) =>
        index === self.findIndex(f => f.name === file.name)
      )
    });
  };

  const removeFile = (indexToRemove) => {
    onFormChange({
      ...formData,
      files: formData.files.filter((_, idx) => idx !== indexToRemove)
    });
  };

  return (
    <div className="addProspectForm">
      <div className="formGroup">
        <label htmlFor="prospectName">Name:</label>
        <input 
          type="text" 
          id="prospectName" 
          value={formData.name}
          onChange={(e) => onFormChange({ ...formData, name: e.target.value })} 
          required 
          className="formInput"
        />
      </div>
      <div className="formGroup">
        <label htmlFor="prospectEmail">Email:</label>
        <input 
          type="email" 
          id="prospectEmail" 
          value={formData.email}
          onChange={(e) => onFormChange({ ...formData, email: e.target.value })} 
          required 
          className="formInput"
        />
      </div>
      <div className="formGroup">
        <label htmlFor="prospectCompany">Company:</label>
        <input 
          type="text" 
          id="prospectCompany" 
          value={formData.company}
          onChange={(e) => onFormChange({ ...formData, company: e.target.value })} 
          required 
          className="formInput"
        />
      </div>
      <div className="formGroup">
        <label htmlFor="prospectFile">Context File (Optional):</label>
        <FileInput
          id="prospectFile"
          onChange={handleFileChange}
          label="Choose File"
          accept=".pdf,.txt,.docx,.doc"
        />
        {formData.files.length > 0 && (
          <div className="selected-files" style={{ marginTop: '40px', color: 'var(--blue0)', border: '1px solid var(--gray3)', borderRadius: '6px', padding: '10px', backgroundColor: 'white' }}>
            <h3 style={{textAlign: 'left'}}>Selected Files:</h3>
            <ul style={{ listStyle: 'none', padding: 0 }}>
              {formData.files.map((file, idx) => (
                <li key={idx} style={{ display: 'flex', alignItems: 'center', marginBottom: '6px' }}>
                  <span style={{ marginRight: '10px' }}>{file.name}</span>
                  <button type="button" onClick={() => removeFile(idx)} style={{ color: '#ff4444', background: 'none', border: 'none', cursor: 'pointer', fontSize: '1.1rem' }}>×</button>
                </li>
              ))}
            </ul>
          </div>
        )}
      </div>
    </div>
  );
};

// --- Prospect Detail View Component ---
const ProspectDetailView = ({ prospect, setSelectedProspect }) => {

  const [prospectFiles, setProspectFiles] = useState([]);
  const [newProspectFiles, setNewProspectFiles] = useState([]);
  const [isCallActive, setIsCallActive] = useState(false);
  const [isEditingProspect, setIsEditingProspect] = useState(false);
  const [isSavingProspect, setIsSavingProspect] = useState(false);
  const [isLoadingCoach, setIsLoadingCoach] = useState(false);
  const [isMeetingPickerOpen, setIsMeetingPickerOpen] = useState(false);
  const [currentProspect, setCurrentProspect] = useState(null);
  
  const { saveProspectMeeting, getProspect, removeZoomMeeting } = useProspects();
  const { handleUploadFiles, fetchFiles, removeFile } = useFiles();
  const { handleNewCall, handleStopLiveCoach, isCallStarting } = useSalesCoach();
  const { formatTime } = useTimes();
  const navigate = useNavigate();
  const { globalUser } = useAuth();

  const handleRemoveFile = async (fileId) => {
    try {
      await removeFile(fileId);
      await fetchFiles(prospect.id);
    } catch (error) {
      console.error('Error removing file:', error);
    }
  }

  const handleRemoveZoomMeeting = async () => {

    try {
      console.log('Removing zoom meeting for prospect:', prospect.id);
      await removeZoomMeeting(prospect.id);
      console.log('Zoom meeting removed for prospect:', prospect.id);
      await fetchProspect(prospect.id);
    } catch (error) {
      console.error('Error removing zoom meeting:', error);
    }
  }


  // Fetch files associated with this prospect
  const handleFetchFiles = useCallback(async (prospectId) => {
    if (!prospectId) return;
    try {
      const data = await fetchFiles(prospectId);
      setProspectFiles(data.files || []);
    } catch (err) {
      console.error('Error fetching files:', err);
    } 
  }, []);

  useEffect(() => {
    if(prospect?.id) {
      handleFetchFiles(prospect.id);
    }
  }, [prospect?.id]);


  const handleAddFile = (event) => {
    const files = Array.from(event.target.files);
    if (!files.length || !prospect?.id) return;

    // Add new files to state, avoiding duplicates
    setNewProspectFiles(prevFiles => {
      const combinedFiles = [...prevFiles, ...files];
      return combinedFiles.filter((file, index, self) => 
        index === self.findIndex(f => f.name === file.name)
      );
    });
  };


  const handleLiveCoach = async () => {

    if(!globalUser) {
      console.log('No global user found');
      return;
    }

    if(!currentProspect?.next_meeting_id) {
      console.log('No meeting found');
      return;
    }

    if(isCallActive) {
      setIsCallActive(false);
      handleStopLiveCoach()
    } else {
      try {
        setIsLoadingCoach(true);
        // await handleNewCall(prospect.id);
        await handleNewCall(prospect.id, globalUser.id, currentProspect.next_meeting_id);
        setIsCallActive(true);
      } catch (error) {
        console.error('Error starting coaching session:', error);
      } finally {
        setIsLoadingCoach(false);
      }
    }
  };

  const handleSaveProspectMeeting = async (meeting) => {
    
    try {
      await saveProspectMeeting(prospect.id, meeting.id, meeting.topic, meeting.start_time);
      await fetchProspect(prospect.id);
      
    } catch (error) {
      console.error('Error saving prospect meeting:', error);
    }
  }


  const handleBackButtonClick = () => {
    setSelectedProspect(null);
  };

  const handleEditProspect = async () => { 

    try {
      if(isEditingProspect) {
        setIsSavingProspect(true);
        await handleUploadFiles(newProspectFiles, 'prospect-files', prospect.id);
  
        handleFetchFiles(prospect.id);
        setIsSavingProspect(false);
      } 
      setIsEditingProspect(!isEditingProspect);
      
    } catch (error) {
      console.error('Error in handleEditProspect:', error);
      setIsSavingProspect(false);
      setIsEditingProspect(false);
      setNewProspectFiles([]);
    }

  };

  const handleCancelEditProspect = () => {
    setIsEditingProspect(false);
    setNewProspectFiles([]);
  };

  const handleLaunchZoomMeeting = () => {
    navigate(`/zoom-client-new/${currentProspect?.next_meeting_id}/${prospect.id}`);
  };

  const fetchProspect = async (prospectId) => {
    const prospectData = await getProspect(prospectId);
    setCurrentProspect(prospectData);
  }


  useEffect(() => {  
    if(prospect) {
      fetchProspect(prospect.id);
    }
  }, [prospect]);

  useEffect(() => {
    if(isCallActive && currentProspect?.next_meeting_id) {
      handleLaunchZoomMeeting()
    }
  }, [isCallActive, currentProspect?.next_meeting_id]);

  return (
    <ContentTemplate
      isCallActive={isCallActive}
      title={`Prospect: ${capitalizeWords(prospect?.name)}`}
      primaryButton={
        !(isLoadingCoach || isCallStarting || isSavingProspect) 
          ? {
              text: isEditingProspect ? 'Save' : 'Edit',
              onClick: handleEditProspect
            }
          : undefined
      }
      secondaryButton={
        !(isLoadingCoach || isCallStarting || isSavingProspect)
          ? {
              text: isEditingProspect ? 'Cancel' : 'Back',
              onClick: isEditingProspect ? handleCancelEditProspect : handleBackButtonClick
            }
          : undefined
      }
      fullWidthButton={
        !(isLoadingCoach || isCallStarting) && currentProspect?.next_meeting_id
          ? {
              text: isCallActive ? 'End Call' : 'Launch Call & Coach', 
              onClick: handleLiveCoach,
              isGreen: true
            }
          : undefined
      }
      displayFullWidthButton={!isEditingProspect}
    >
      {(isLoadingCoach || isCallStarting) ? (
        <div style={{
          display: 'flex',
          flexDirection: 'column',
          justifyContent: 'center',
          alignItems: 'center',
          minHeight: '200px'
        }}>
          <SaysoLoader textArray={['Starting coaching session', 'Getting prospect data', 'Creating context', 'Getting ready to coach', 'Almost done']} />
        </div>
      ) : isSavingProspect ? (
        <SaysoLoader textArray={['Saving prospect', 'Almost done', 'Just a moment']} /> 
      ) : (
        <div className="prospectDetailView" style={{ width: 500, margin: '0 auto' }}>
          <div className="formGroup">
            <label>Name:</label>
            <div className="formInput" style={{ background: '#fff', border: '1px solid #e3e8f0', borderRadius: '10px', color: '#4a5568', fontSize: '1rem', marginBottom: 0, width: '100%', boxSizing: 'border-box' }}>{capitalizeWords(prospect?.name)}</div>
          </div>
          <div className="formGroup">
            <label>Email:</label>
            <div className="formInput" style={{ background: '#fff', border: '1px solid #e3e8f0', borderRadius: '10px', color: '#4a5568', fontSize: '1rem', marginBottom: 0, width: '100%', boxSizing: 'border-box' }}>{prospect?.email}</div>
          </div>
          <div className="formGroup" style={{ marginTop: '30px' }}>
            <label>Files:</label>
            {
              prospectFiles.length > 0 ? (
                <FileListContainer 
                  files={prospectFiles} 
                  title="Uploaded Files"
                  onRemoveFile={handleRemoveFile}
                  isEditing={isEditingProspect}
                />
              ) : (
                <p>No files uploaded yet.</p>
              )
            }
            {
              isEditingProspect && (
                <>
                  {newProspectFiles.length > 0 && (
                    <FileListContainer 
                      files={newProspectFiles.map(file => ({ name: file.name }))}
                      title="New Files (not uploaded yet)"
                    />
                  )}
                  <FileInput
                    id="prospectFileInput"
                    onChange={handleAddFile}
                    style={{ marginTop: '10px' }}
                    label="Select Files"
                    accept=".pdf,.txt,.docx,.doc"
                    multiple
                  />
                </>
              )
            }
          </div>
        </div>
      )}
    {!isLoadingCoach && !isCallStarting && !isSavingProspect && (
      <div className='meeting-picker-container' style={{display: 'flex', flexDirection: 'column', justifyContent: 'center', alignItems: 'start', width: '100%', marginTop: '20px'}}>
        <p style={{fontSize: '1rem', color: 'var(--gray4)', fontWeight: '500', marginRight: '10px'}}>Next Meeting:</p>
        {
          !currentProspect?.next_meeting_id ? (
            <>
              <button style={{backgroundColor: 'var(--blueZoom)', color: 'white', width: '100%', padding: '10px 20px', borderRadius: '5px', cursor: 'pointer', border: 'none', height: '40px', fontSize: '0.96rem', marginBottom: '50px'}} onClick={() => setIsMeetingPickerOpen(true)}>Pick a Zoom Meeting</button>
            </>
          ) : (
            <>
              <div style={{ display: 'flex', alignItems: 'start', justifyContent: 'space-between' }}>
                <div>
                  <div style={{ display: 'flex', alignItems: 'center' }}>
                    <p style={{fontSize: '1rem', color: 'var(--gray4)', fontWeight: '500', marginRight: '10px', textAlign: 'left', margin: 0}}>{currentProspect.next_meeting_topic}</p>
                    {isEditingProspect && (
                      <button 
                        type="button" 
                        onClick={handleRemoveZoomMeeting} 
                        style={{ 
                          color: '#ff4444', 
                          background: 'none', 
                          border: 'none', 
                          cursor: 'pointer', 
                          fontSize: '1.1rem',
                          marginLeft: '5px'
                        }}
                      >
                        ×
                      </button>
                    )}
                  </div>
                  <p style={{fontSize: '0.9rem', color: 'var(--gray4)', fontWeight: '500', marginRight: '10px', marginTop: '0px', textAlign: 'left'}}>{formatTime(currentProspect.next_meeting_start_time)}</p>
                </div>
              </div>
            </>
          )
        }
      </div>
    )}

    { 
      isMeetingPickerOpen && (
        <MeetingPickerModal isOpen={isMeetingPickerOpen} onClose={() => setIsMeetingPickerOpen(false)} prospectId={prospect.id} handleSaveProspectMeeting={handleSaveProspectMeeting}/>
      )
    }
    </ContentTemplate>
  );
};

// Main Dashboard Component
const Dashboard = () => {
  const { prospects, loading, setProspects } = useProspectsContext();
  const { globalUser, signOut } = useAuth();
  const [selectedProspect, setSelectedProspect] = useState(null);
  const [isAddingProspect, setIsAddingProspect] = useState(false);
  const [isAccountSettingsOpen, setIsAccountSettingsOpen] = useState(false);
  const [formData, setFormData] = useState({ name: '', email: '', company: '', files: [] });
  const [isEditingAccountSettings, setIsEditingAccountSettings] = useState(false);
  const [newAccountFiles, setNewAccountFiles] = useState([]);
  const [accountFiles, setAccountFiles] = useState([]);
  const [isSavingAccountSettings, setIsSavingAccountSettings] = useState(false);
  const [isLoadingFiles, setIsLoadingFiles] = useState(false);

  const {  handleNewProspect } = useProspects()
  const { handleUploadFiles, fetchFiles, removeFile } = useFiles();
  const { setCurrentInsight, isChecklistVisible, setIsChecklistVisible } = useSalesCoachContext();

  const handleRemoveFile = async (fileId) => {
    try {
      await removeFile(fileId);
    } catch (error) {
      console.error('Error removing file:', error);
    }
  }


  const handleAccountSettingsClick = () => {
    setSelectedProspect(null);
    setIsAddingProspect(false);
    setIsAccountSettingsOpen(true);
  };

  const handleAddProspectClick = () => {
    setFormData({ name: '', email: '', files: [] });
    setIsAddingProspect(true);
  };

  const handleSelectProspect = (prospect) => {
    setSelectedProspect(prospect);
    setIsAddingProspect(false);
  };

  const handleAddProspect = async (prospectData) => {
    try {
      // Add account_id from globalUser
      const { files, ...prospectFields } = prospectData;
      const newProspect = await handleNewProspect(
        { ...prospectFields, account_id: globalUser.id },
        files
      );
      setProspects(prev => [newProspect, ...prev]);
      setIsAddingProspect(false);
      setFormData({ name: '', email: '', files: [] }); // Reset form data
    } catch (err) {
      console.error('Error adding prospect:', err);
      alert('Failed to add prospect');
    }
  };

  const handleCancelAddProspect = () => {
    setIsAddingProspect(false);
  };

  const handleAccountFileChange = (event) => {
    const newFiles = Array.from(event.target.files);
    setNewAccountFiles(prevFiles => {
      // Combine previous files with new files, removing duplicates
      const combinedFiles = [...(prevFiles || []), ...newFiles];
      return combinedFiles.filter((file, index, self) => 
        index === self.findIndex(f => f.name === file.name)
      );
    });
  };

  const handleEditAccountSettings = () => {
    if(isEditingAccountSettings) {  
      // Save account settings
      handleSaveAccountSettings();
      setIsEditingAccountSettings(false);
    } else {
      setIsEditingAccountSettings(true);
    }
  };

  const handleCancelEditAccountSettings = () => {
    setIsEditingAccountSettings(false);
    setNewAccountFiles([]);
  };

  const handleBackButtonClick = () => {
    setIsAccountSettingsOpen(false);
    setNewAccountFiles([]);
  };  


  const handleFetchAccountFiles = useCallback(async (accountId) => {
    setIsLoadingFiles(true);
    try {
      const data = await fetchFiles(accountId);
      setAccountFiles(data.files || []);
    } catch (error) {
      console.error('Error fetching account files:', error);
    } finally {
      setIsLoadingFiles(false);
    }
  }, [fetchFiles]);

  const handleSaveAccountSettings = async () => {

    try {
      setIsSavingAccountSettings(true);
      if (newAccountFiles.length > 0) {
        await handleUploadFiles(newAccountFiles, 'account-files', globalUser.id);
        setNewAccountFiles([]);
      }
      await handleFetchAccountFiles(globalUser.id);
      setIsEditingAccountSettings(false);

      setIsSavingAccountSettings(false);
    } catch (error) {
      setIsSavingAccountSettings(false);
      console.error('Error saving account settings:', error);
      alert('Failed to save account settings');
    }
  };



  useEffect(() => {
    if(globalUser?.id) {
      handleFetchAccountFiles(globalUser.id);
    }
  }, [globalUser?.id]);


  return (
    <div className="dashboardContainer">
      <div className="sidebar">
        <div>
          <h2 className="sidebarTitle">Prospects</h2>
          <ul className="prospectList">
            
            {loading ? (
              <li className="prospectItem">Loading prospects...</li>
            ) : prospects.length === 0 ? (
              <>
                <li style={{fontSize:'0.9rem', color:'var(--blue0)', margin:'20px 0'}}>No prospects found. Add your first prospect to get started!</li>
                <li className='flex items-center gap-2' style={{marginTop:'30px'}}>
                  <BtnPrimary text="Add Prospect" onClick={handleAddProspectClick} />
                </li>
              </>
            ) : (
              prospects.map(prospect => (
                <li 
                  key={prospect.id}
                  className={`prospectItem ${selectedProspect?.id === prospect.id ? 'selected' : ''}`}
                  onClick={() => handleSelectProspect(prospect)}
                >
                  {capitalizeWords(prospect.name)}
                </li>
              ))
            )}
          </ul>
        </div>
        <div> 
          <span style={{height: '1px', width: '100%', backgroundColor:'var(--blue1)', display: 'block', margin:'10px 0', opacity: '0.4'}}></span>
          <div style={{ color:'var(--blue0)', fontSize:'0.95rem', textAlign: 'left', padding:'0 6px', display:'flex', alignItems:'start', justifyContent:'center', flexDirection: 'column'}}>
            <p style={{marginTop:'10px', fontWeight:'700'}}>{globalUser?.name} {globalUser?.lastname}</p>
            <p style={{marginTop:'-5px'}}>{globalUser?.email}</p>
          </div>
          <span style={{height: '1px', width: '100%', backgroundColor:'var(--blue1)', display: 'block', margin:'10px 0', opacity: '0.4'}}></span>
          <BtnMain text="Account Settings" onClick={handleAccountSettingsClick} />
          <BtnMain text="Sign Out" onClick={signOut} />
          <BtnMain text="Test Checklist" onClick={() => setIsChecklistVisible(!isChecklistVisible)} />
          <BtnMain text="Regular Insight" onClick={() => setCurrentInsight({message: "I hope your kids are excited for school! How's the planning going? I hope your kids are excited for school! How's the planning going?", isIceBreaker: false})} />
        </div>
      </div>

      <div className="mainContent">

      {isAddingProspect ? (
          <ContentTemplate
            title="Add New Prospect"
            primaryButton={{
              text: "Save Prospect",
              onClick: () => {
                if (!formData.name || !formData.email) {
                  alert('Please fill in name and email.');
                  return;
                }
                handleAddProspect(formData);
              }
            }}
            secondaryButton={{
              text: "Cancel",
              onClick: handleCancelAddProspect
            }}
          >
            <AddProspectForm
              formData={formData}
              onFormChange={setFormData}
            />
          </ContentTemplate>
        ) : selectedProspect ? (
          <ProspectDetailView
            prospect={selectedProspect}
            setSelectedProspect={setSelectedProspect}
          />
        ) : isAccountSettingsOpen ? (
          <ContentTemplate
            title={isEditingAccountSettings ? 'Edit Account Settings' : 'Account Settings'}
            primaryButton={
              !isSavingAccountSettings ? {
                text: isEditingAccountSettings ? 'Save' : 'Edit',
                onClick: handleEditAccountSettings,
                isLoading: isSavingAccountSettings
              } : undefined
            }
            secondaryButton={
              !isSavingAccountSettings ? {
                text: isEditingAccountSettings ? 'Cancel' : 'Back',
                onClick: isEditingAccountSettings ? handleCancelEditAccountSettings : handleBackButtonClick
              } : undefined
            }
          >
            {isSavingAccountSettings ? 
              <SaysoLoader textArray={['Saving account settings', 'Almost done', 'Just a moment']} /> :
              // <Loader message="Saving account settings..." /> :
              isLoadingFiles ? 
                <SaysoLoader textArray={['Loading files', 'Almost done', 'Just a moment']} /> :
                <div className="accountSettingsView" style={{ width: 500, margin: '0 auto' }}>
                  <div className="formGroup">
                    <label>Name:</label>
                    <div className="formInput" style={{ background: '#fff', border: '1px solid #e3e8f0', borderRadius: '10px', color: '#4a5568', fontSize: '1rem', marginBottom: 0, width: '100%', boxSizing: 'border-box', textAlign: 'left' }}>
                      {globalUser?.name}
                    </div>
                  </div>
                  <div className="formGroup">
                    <label>Email:</label>
                    <div className="formInput" style={{ background: '#fff', border: '1px solid #e3e8f0', borderRadius: '10px', color: '#4a5568', fontSize: '1rem', marginBottom: 0, width: '100%', boxSizing: 'border-box', textAlign: 'left' }}>
                      {globalUser?.email}
                    </div>
                  </div>
                  <div className="formGroup">
                    <label>Company:</label>
                    <div className="formInput" style={{ background: '#fff', border: '1px solid #e3e8f0', borderRadius: '10px', color: '#4a5568', fontSize: '1rem', marginBottom: 0, width: '100%', boxSizing: 'border-box', textAlign: 'left' }}>
                      {globalUser?.company}
                    </div>
                  </div>
                  <div className="formGroup" style={{ marginTop: '30px' }}>
                    <label>Context Files:</label>
                    {
                      accountFiles && accountFiles?.length > 0 ? (
                        <FileListContainer 
                          files={accountFiles} 
                          title="Uploaded Files"  
                          onRemoveFile={handleRemoveFile}
                          isEditing={isEditingAccountSettings}
                        />
                      ) : (
                        <p style={{fontSize: '0.9rem', color: 'var(--blue0)'}}>No files uploaded yet.</p>
                      )
                    }
                    {
                      isEditingAccountSettings && (
                        <>
                          {newAccountFiles.length > 0 && (
                            <FileListContainer 
                              files={newAccountFiles.map(file => ({ name: file.name }))}
                              title="New Files (not uploaded yet)"
                            />
                          )}
                          <FileInput
                            id="accountFileInput"
                            onChange={handleAccountFileChange}
                            style={{ marginTop: '10px' }}
                            label="Select Files"
                            accept=".pdf,.txt,.docx,.doc"
                            multiple
                          />
                        </>
                      )
                    }
                  </div>
                  <div className="formGroup">
                    <label>Zoom Account:</label>
                    
                    <div style={{marginTop: '20px', marginBottom: '30px', display: 'flex', justifyContent: 'center', alignItems: 'center', width: '100%'}}>
                      <ConnectZoomButton />
                    </div>
                  </div>
                  
                </div>
            }
          </ContentTemplate>
        ) : (
          <div style={{width: '500px', margin: '0 auto'}}>
            <ContentTemplate centerBody={true}>
              <div style= {{width:'100%', display:'flex', flexDirection:'column', alignItems:'center', justifyContent:'center'}}>
                <h1>Welcome to Sayso</h1>
                <p style={{textAlign: 'center', fontSize: '1.2rem', fontWeight: '500', color: 'var(--blue0)'}}>Select a prospect or add a new one.</p>
                <div style={{marginTop: '40px', width: '500px'}}>
                  <BtnPrimary text="Add Prospect" onClick={handleAddProspectClick} />
                </div>
              </div>
            </ContentTemplate>
          </div>
        )}
      </div>
    </div>
  );
};

export default Dashboard; 