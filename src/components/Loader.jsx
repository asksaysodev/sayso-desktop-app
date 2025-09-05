import React from 'react';
import '../styles/Loader.css';

const Loader = ({ message = 'Sayso' }) => {
  return (
    <div className="loaderView">
      <div className="loadingContainer">
        <div className="loader"></div>
        <p>{message}</p>
      </div>
    </div>
  );
};

export default Loader; 