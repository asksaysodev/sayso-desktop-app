import type { BrowserWindow as BrowserWindowType } from 'electron';
import { screen } from 'electron';
import { WINDOW_CONFIG } from './windowConfig';
import { IS_MAC, IS_WINDOWS } from './platform';

class WindowManager {
  static getActiveDisplay() {
    const cursorPoint = screen.getCursorScreenPoint();
    return screen.getDisplayNearestPoint(cursorPoint);
  }

  /**
   * Title-bar options for the three framed windows (splash, onboarding,
   * app settings).
   *
   * `titleBarStyle: 'hiddenInset'` is macOS-only.
   *
   * Windows instead gets Window Controls Overlay: `'hidden'` drops the title
   * bar entirely and `titleBarOverlay` paints just the minimize/close buttons
   * over our own content in the top-right, so the page owns the full window
   * height exactly as it does under `hiddenInset` on macOS.
   *
   * `titleBarOverlay` is not Windows-only. Its `color`/`symbolColor` are
   * (`@platform win32,linux`), but `height` applies on macOS too — it sizes the
   * band the traffic lights are centred in. Both platforms therefore get
   * `height`, and only Windows gets the colours; dropping `height` on macOS
   * would silently move App Settings' traffic lights off the position its 52px
   * top paddings were tuned against.
   */
  static getTitleBarConfig(color: string, height: number) {
    if (IS_MAC) {
      return { titleBarStyle: 'hiddenInset' as const, titleBarOverlay: { height } };
    }

    return {
      titleBarStyle: 'hidden' as const,
      titleBarOverlay: { color, symbolColor: '#FFFFFF', height },
    };
  }

  /**
   * Resize a window, keeping it anchored where it is.
   *
   * Use this instead of `setSize()` on any window created `resizable: false`.
   * On Windows such a window can grow but never shrink: Win32 holds a
   * minimum-size floor that ratchets up to the largest size the window has
   * held (starting at its creation size) and silently drops any smaller
   * request. `setBounds()` isn't subject to that floor. macOS is unaffected
   * either way, so there is one code path.
   */
  static setWindowSize(win: BrowserWindowType, width: number, height: number) {
    const { x, y } = win.getBounds();
    win.setBounds({ x, y, width, height });
  }

  /**
   * Where the tray menu popover should sit for a given size.
   *
   * Takes the height as an argument rather than reading it off the window so a
   * caller can compute the destination for a size the window does not have yet
   * and apply both in a single `setBounds`. That matters on Windows, where the
   * popover is anchored by its **bottom** edge (it opens upward, above the
   * taskbar): resizing from the current top-left first and repositioning
   * afterwards leaves one frame of wrong geometry, with the menu extended down
   * over the taskbar before it snaps back.
   */
  static calculateTrayMenuPosition(
    trayBounds: { x: number; y: number; width: number; height: number },
    width: number,
    height: number,
  ) {
    // macOS: tray.getBounds() can report the primary display even when the icon
    // was clicked on a secondary display's menu bar, so the cursor is the more
    // reliable signal there. Everywhere else trayBounds is trustworthy, and the
    // cursor's display would be the wrong one to clamp against whenever the
    // tray sits on another monitor's taskbar.
    const cursorPoint = screen.getCursorScreenPoint();
    const { workArea } = IS_MAC
      ? screen.getDisplayNearestPoint(cursorPoint)
      : screen.getDisplayMatching(trayBounds);

    let x: number;
    let y: number;

    if (IS_MAC) {
      // Center horizontally around the cursor (where the icon was clicked),
      // and place just below this display's menu bar.
      x = Math.round(cursorPoint.x - width / 2);
      y = Math.round(workArea.y + 5);

      // Clamp to this display's bounds
      if (x + width > workArea.x + workArea.width) {
        x = workArea.x + workArea.width - width - 5;
      }
      if (x < workArea.x) {
        x = workArea.x + 5;
      }
    } else {
      // Windows: above the taskbar. Linux: below the icon. Both centred on it.
      x = Math.round(trayBounds.x + (trayBounds.width / 2) - (width / 2));
      y = IS_WINDOWS
        ? Math.round(trayBounds.y - height - 5)
        : Math.round(trayBounds.y + trayBounds.height + 5);

      // A left- or right-docked Windows taskbar puts the tray icon against a
      // screen edge, which without this hangs the menu ~90px off-screen.
      x = Math.min(Math.max(x, workArea.x), workArea.x + workArea.width - width);
      y = Math.min(y, workArea.y + workArea.height - height);
    }

    // Top-docked taskbar: the Windows branch anchors the bottom edge above the
    // tray, which would otherwise put Quit / Log Out off the top of the screen.
    y = Math.max(y, workArea.y);

    return { x, y };
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
      WindowManager.setWindowSize(coachWindow, validatedWidth, validatedHeight);
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
      // Prevent user from maximizing or resizing the coach window
      resizable: WINDOW_CONFIG.COACH.RESIZABLE,
      maximizable: WINDOW_CONFIG.COACH.MAXIMIZABLE,
      fullscreenable: WINDOW_CONFIG.COACH.FULLSCREENABLE,
      minimizable: WINDOW_CONFIG.COACH.MINIMIZABLE,
      hasShadow: WINDOW_CONFIG.COACH.HAS_SHADOW,
      // Windows gives every window its own taskbar button, where the macOS Dock
      // shows one icon per app. Without this the overlays would each add a
      // button of their own.
      skipTaskbar: IS_WINDOWS,
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
      resizable: WINDOW_CONFIG.PLAYBOOK.RESIZABLE,
      maximizable: WINDOW_CONFIG.PLAYBOOK.MAXIMIZABLE,
      minimizable: WINDOW_CONFIG.PLAYBOOK.MINIMIZABLE,
      fullscreenable: WINDOW_CONFIG.PLAYBOOK.FULLSCREENABLE,
      hasShadow: WINDOW_CONFIG.PLAYBOOK.HAS_SHADOW,
      skipTaskbar: IS_WINDOWS,
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
