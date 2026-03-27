import type { BrowserWindow as BrowserWindowType } from 'electron';
import { screen } from 'electron';
import { WINDOW_CONFIG } from './windowConfig';

class WindowManager {
  /**
   * Calculate coach window position (centered horizontally, offset from top)
   * @param {number} windowWidth - Width of the window
   * @returns {Object} Position coordinates {x, y}
   */
  static calculateCoachWindowPosition(windowWidth = WINDOW_CONFIG.COACH.DEFAULT_WIDTH) {
    const primaryDisplay = screen.getPrimaryDisplay();
    const { width: screenWidth } = primaryDisplay.workAreaSize;
    
    return {
      x: Math.round((screenWidth - windowWidth) / 2),
      y: WINDOW_CONFIG.COACH.OFFSET_Y
    };
  }

  /**
   * Resize coach window with validation
   * @param {Object} coachWindow - The coach window instance
   * @param {number} width - Desired width
   * @param {number} height - Desired height
   * @returns {boolean} Success status
   */
  static resizeCoachWindow(coachWindow: BrowserWindowType, width: number, height: number) {
    if (!coachWindow || coachWindow.isDestroyed()) {
      console.warn('WindowManager: Cannot resize - coach window is null or destroyed');
      return false;
    }

    // Validate dimensions
    const validatedWidth = Math.max(
      WINDOW_CONFIG.COACH.MIN_WIDTH,
      Math.min(WINDOW_CONFIG.COACH.MAX_WIDTH, width)
    );
    
    const validatedHeight = Math.max(
      WINDOW_CONFIG.COACH.MIN_HEIGHT,
      Math.min(WINDOW_CONFIG.COACH.MAX_HEIGHT, height)
    );

    try {
      coachWindow.setSize(validatedWidth, validatedHeight);
      console.log(`WindowManager: Coach window resized to ${validatedWidth}x${validatedHeight}`);
      return true;
    } catch (error) {
      console.error('WindowManager: Error resizing coach window:', error);
      return false;
    }
  }

  /**
   * Get coach window configuration
   * @param {number} customWidth - Optional custom width
   * @returns {Object} Window configuration object
   */
  static getCoachWindowConfig(customWidth = null) {
    const width = customWidth || WINDOW_CONFIG.COACH.DEFAULT_WIDTH;
    const position = this.calculateCoachWindowPosition(width);
    
    return {
      width,
      height: WINDOW_CONFIG.COACH.DEFAULT_HEIGHT,
      x: WINDOW_CONFIG.COACH.OFFSET_X,
      y: WINDOW_CONFIG.COACH.OFFSET_Y,
      frame: WINDOW_CONFIG.COACH.FRAME,
      transparent: WINDOW_CONFIG.COACH.TRANSPARENT,
      alwaysOnTop: WINDOW_CONFIG.COACH.ALWAYS_ON_TOP,
      devTools: WINDOW_CONFIG.COACH.DEV_TOOLS,
      visibleOnAllWorkspaces: WINDOW_CONFIG.COACH.VISIBLE_ON_ALL_WORKSPACES,
      // Prevent user from maximizing or resizing the coach window
      resizable: WINDOW_CONFIG.COACH.RESIZABLE,
      maximizable: WINDOW_CONFIG.COACH.MAXIMIZABLE,
      fullscreenable: WINDOW_CONFIG.COACH.FULLSCREENABLE,
      minimizable: WINDOW_CONFIG.COACH.MINIMIZABLE,
      hasShadow: WINDOW_CONFIG.COACH.HAS_SHADOW
    };
  }
  
  static getCoachSettingsWindowConfig() {
      return {
          width: 800,
          height: 600,
          // alwaysOnTop: true,
          visibleOnAllWorkspaces: true,
          resizable: false,
          minimizable: false,
          maximizable: false,
          fullscreenable: false,
      };
  }
}

export { WindowManager };
