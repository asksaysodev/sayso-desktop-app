import React from 'react';
import '../styles/Loader.css';

const Loader = ({ message = 'Loading...' }) => {
  return (
    <div className="loadingContainer">
      <div className="loadingSpinner"></div>
      <p>{message}</p>
    </div>
  );
};

export default Loader; 