import React, { useState, useEffect } from 'react';
import '../styles/ContentTemplate.css';
import BtnSecondary from './BtnSecondary';
import BtnPrimary from './BtnPrimary';
import FileInput from './FileInput';

const ContentTemplate = ({ 
  title, 
  children, 
  primaryButton, 
  secondaryButton,
  className = '',
  backButton,
  centerBody = false,
  fullWidthButton,
  displayFullWidthButton = true,
  isCallActive = false,
  connections
}) => {
  const [selectedFiles, setSelectedFiles] = useState([]);

  const handleFileChange = (event) => {
    const newFiles = Array.from(event.target.files);
    setSelectedFiles(prevFiles => [...prevFiles, ...newFiles]);
  };

  const removeFile = (indexToRemove) => {
    setSelectedFiles(prevFiles => prevFiles.filter((_, index) => index !== indexToRemove));
  };


  return (
    <div className={`content-template ${className}`} style={{ position: 'relative' }}>
      {backButton}
      <div className="content-header">
        <h1>{title}</h1>
      </div>
      
      <div 
        className="content-body" 
        style={centerBody ? { display: 'flex', flexDirection: 'column', alignItems: 'center', justifyContent: 'center', minHeight: '60vh' } : {}}
      >
        {children}
      </div>

      {(primaryButton || secondaryButton || fullWidthButton) && (
        <div className="content-actions-container" style={{display: 'flex', flexDirection: 'column', gap: '10px', width: '500px'}}>
          <div className="content-actions" style={{width: '100%'}}>
            <div style={{ display: 'flex', gap: '10px', width: '100%' }}>
              {secondaryButton && (
                <div style={{ width: primaryButton ? '50%' : '100%' }}>
                  <BtnSecondary
                    text={secondaryButton.text}
                    onClick={secondaryButton.onClick}
                  />
                </div>
              )}
              {primaryButton && (
                <div style={{ width: secondaryButton ? '50%' : '100%' }}>
                  <BtnPrimary
                    text={primaryButton.text}
                    onClick={primaryButton.onClick}
                  />
                </div>
              )}
            </div>
          </div>
          <div className="content-actions width-100">
            <div style={{ display: 'flex', width: '100%', marginTop: '20px' }}>
              {fullWidthButton && displayFullWidthButton && (
                <div style={{ width: '100%' }}>
                  <BtnPrimary
                    text={fullWidthButton.text}
                    onClick={fullWidthButton.onClick}
                    isGreen={fullWidthButton.isGreen}
                    isCallActive={isCallActive}
                  />
                </div>
              )}
              {connections && (
                <div style={{ width: '100%' }}>
                  {connections}
                </div>
              )}
            </div>
          </div>
        </div>
      )}
    </div>
  );
};

export default ContentTemplate; 