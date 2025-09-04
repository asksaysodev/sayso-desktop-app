import React from 'react';
import { CoachWindowProvider } from '../context/CoachWindowContext';
import CoachWindowMain from './components/CoachWindowMain';

function CoachWindowApp() {
  return (
    <CoachWindowProvider>
      <CoachWindowMain />
    </CoachWindowProvider>
  );
}

export default CoachWindowApp;
