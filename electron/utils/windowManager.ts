import type { BrowserWindow as BrowserWindowType } from 'electron';
import { screen } from 'electron';
import { WINDOW_CONFIG } from './windowConfig';

class WindowManager {
  static getActiveDisplay() {
    const cursorPoint = screen.getCursorScreenPoint();
    return screen.getDisplayNearestPoint(cursorPoint);
  }

  static calculateCoachWindowPosition() {
    const { workArea } = WindowManager.getActiveDisplay();
    return {
      x: workArea.x + WINDOW_CONFIG.COACH.OFFSET_X,
      y: workArea.y + WINDOW_CONFIG.COACH.OFFSET_Y,
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

    // Cap height to the available work area on whichever display the window is on,
    // leaving a small margin so the window never bleeds under the dock/taskbar.
    const { workArea } = screen.getDisplayMatching(coachWindow.getBounds());
    const windowY = coachWindow.getBounds().y;
    const screenMaxHeight = workArea.y + workArea.height - windowY - 20;
    const effectiveMaxHeight = Math.min(WINDOW_CONFIG.COACH.MAX_HEIGHT, screenMaxHeight);

    // Validate dimensions
    const validatedWidth = Math.max(
      WINDOW_CONFIG.COACH.MIN_WIDTH,
      Math.min(WINDOW_CONFIG.COACH.MAX_WIDTH, width)
    );

    const validatedHeight = Math.max(
      WINDOW_CONFIG.COACH.MIN_HEIGHT,
      Math.min(effectiveMaxHeight, height)
    );

    try {
      coachWindow.setSize(validatedWidth, validatedHeight);
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
    const { x, y } = this.calculateCoachWindowPosition();

    return {
      width,
      height: WINDOW_CONFIG.COACH.DEFAULT_HEIGHT,
      x,
      y,
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
      hasShadow: WINDOW_CONFIG.COACH.HAS_SHADOW,
      // Fully transparent backing so there's no white flash when the renderer
      // isn't painted over it (e.g. during the native close animation).
      // No vibrancy here: NSVisualEffectView blurs the whole window rect, but
      // this window's content is a rounded pill smaller than the window
      // bounds, so vibrancy shows up as a visible frosted box around it.
      backgroundColor: '#00000000'
    };
  }
  
  /**
   * Calculate playbook window position to the right of the coach window
   * with a fixed gap. Clamps to the coach window's display work area so the
   * playbook never lands off-screen.
   */
  static calculatePlaybookWindowPosition(coachBounds: { x: number; y: number; width: number; height: number }) {
    const display = screen.getDisplayMatching(coachBounds);
    const workArea = display.workArea;

    const desiredX = coachBounds.x + coachBounds.width + WINDOW_CONFIG.PLAYBOOK.GAP_X;
    const maxX = workArea.x + workArea.width - WINDOW_CONFIG.PLAYBOOK.WIDTH;
    const x = Math.max(workArea.x, Math.min(desiredX, maxX));

    const maxY = workArea.y + workArea.height - WINDOW_CONFIG.PLAYBOOK.HEIGHT;
    const y = Math.max(workArea.y, Math.min(coachBounds.y, maxY));

    return { x: Math.round(x), y: Math.round(y) };
  }

  static calculateStandalonePlaybookWindowPosition() {
    const { workArea } = WindowManager.getActiveDisplay();
    const x = workArea.x + workArea.width - WINDOW_CONFIG.PLAYBOOK.WIDTH - 20;
    const y = workArea.y + 20;
    return { x: Math.round(x), y: Math.round(y) };
  }

  static getPlaybookWindowConfig(coachBounds?: { x: number; y: number; width: number; height: number }) {
    const { x, y } = coachBounds
      ? this.calculatePlaybookWindowPosition(coachBounds)
      : this.calculateStandalonePlaybookWindowPosition();

    return {
      width: WINDOW_CONFIG.PLAYBOOK.WIDTH,
      height: WINDOW_CONFIG.PLAYBOOK.HEIGHT,
      minWidth: WINDOW_CONFIG.PLAYBOOK.MIN_WIDTH,
      minHeight: WINDOW_CONFIG.PLAYBOOK.MIN_HEIGHT,
      x,
      y,
      frame: WINDOW_CONFIG.PLAYBOOK.FRAME,
      transparent: WINDOW_CONFIG.PLAYBOOK.TRANSPARENT,
      alwaysOnTop: WINDOW_CONFIG.PLAYBOOK.ALWAYS_ON_TOP,
      visibleOnAllWorkspaces: WINDOW_CONFIG.PLAYBOOK.VISIBLE_ON_ALL_WORKSPACES,
      resizable: WINDOW_CONFIG.PLAYBOOK.RESIZABLE,
      maximizable: WINDOW_CONFIG.PLAYBOOK.MAXIMIZABLE,
      minimizable: WINDOW_CONFIG.PLAYBOOK.MINIMIZABLE,
      fullscreenable: WINDOW_CONFIG.PLAYBOOK.FULLSCREENABLE,
      hasShadow: WINDOW_CONFIG.PLAYBOOK.HAS_SHADOW,
      // See getCoachWindowConfig: fully transparent backing avoids the white
      // close flash. No vibrancy for the same reason (visible frosted box).
      backgroundColor: '#00000000'
    };
  }

  static getAppSettingsWindowConfig() {
      const desiredWidth = 850;
      const desiredHeight = 610;
      const { workArea } = WindowManager.getActiveDisplay();

      // Clamp to the active display's work area so a small/short secondary
      // display can't push this fixed-size (resizable: false) window
      // partially or fully off-screen.
      const width = Math.min(desiredWidth, workArea.width);
      const height = Math.min(desiredHeight, workArea.height);

      return {
          width,
          height,
          x: Math.round(workArea.x + (workArea.width - width) / 2),
          y: Math.round(workArea.y + (workArea.height - height) / 2),
          visibleOnAllWorkspaces: true,
          resizable: false,
          minimizable: false,
          maximizable: false,
          fullscreenable: false,
          // Matches the app's dark UI so there's no white flash when the
          // renderer isn't painted over the native backing (e.g. on close).
          backgroundColor: '#02192f',
      };
  }
}

export { WindowManager };
