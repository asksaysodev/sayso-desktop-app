import React from 'react';
import { CoachWindowProvider } from '../context/CoachWindowContext';
import CoachWindowMain from './components/CoachWindowMain';

/**
 * Coach window root component. Also wrapped with CoachWindowProvider since this
 * renders in a separate Electron window with its own React context tree.
 * @returns {JSX.Element}
 */
function CoachWindowApp() {
  return (
    <CoachWindowProvider>
      <CoachWindowMain />
    </CoachWindowProvider>
  );
}

export default CoachWindowApp;
