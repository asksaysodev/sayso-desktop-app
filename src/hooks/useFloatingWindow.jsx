import { useFloatingWindow } from '../context/FloatingWindowContext';

export const useFloatingWindowControls = () => {
  const {
    isVisible,
    windowPosition,
    showWindow,
    hideWindow,
    toggleWindow,
    updatePosition
  } = useFloatingWindow();

  return {
    isVisible,
    windowPosition,
    showWindow,
    hideWindow,
    toggleWindow,
    updatePosition
  };
}; 