import React from 'react';
import CoachWindowMain from './components/CoachWindowMain';

/**
 * Coach window root component. Also wrapped with CoachWindowProvider since this
 * renders in a separate Electron window with its own React context tree.
 * @returns {JSX.Element}
 */
function CoachWindowApp() {
  return (
      <CoachWindowMain />
  );
}

export default CoachWindowApp;
