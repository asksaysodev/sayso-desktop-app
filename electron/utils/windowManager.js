"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.WindowManager = void 0;
const electron_1 = require("electron");
const windowConfig_1 = require("./windowConfig");
class WindowManager {
    /**
     * Calculate coach window position (centered horizontally, offset from top)
     * @param {number} windowWidth - Width of the window
     * @returns {Object} Position coordinates {x, y}
     */
    static calculateCoachWindowPosition(windowWidth = windowConfig_1.WINDOW_CONFIG.COACH.DEFAULT_WIDTH) {
        const primaryDisplay = electron_1.screen.getPrimaryDisplay();
        const { width: screenWidth } = primaryDisplay.workAreaSize;
        return {
            x: Math.round((screenWidth - windowWidth) / 2),
            y: windowConfig_1.WINDOW_CONFIG.COACH.OFFSET_Y
        };
    }
    /**
     * Resize coach window with validation
     * @param {Object} coachWindow - The coach window instance
     * @param {number} width - Desired width
     * @param {number} height - Desired height
     * @returns {boolean} Success status
     */
    static resizeCoachWindow(coachWindow, width, height) {
        if (!coachWindow || coachWindow.isDestroyed()) {
            console.warn('WindowManager: Cannot resize - coach window is null or destroyed');
            return false;
        }
        // Validate dimensions
        const validatedWidth = Math.max(windowConfig_1.WINDOW_CONFIG.COACH.MIN_WIDTH, Math.min(windowConfig_1.WINDOW_CONFIG.COACH.MAX_WIDTH, width));
        const validatedHeight = Math.max(windowConfig_1.WINDOW_CONFIG.COACH.MIN_HEIGHT, Math.min(windowConfig_1.WINDOW_CONFIG.COACH.MAX_HEIGHT, height));
        try {
            coachWindow.setSize(validatedWidth, validatedHeight);
            console.log(`WindowManager: Coach window resized to ${validatedWidth}x${validatedHeight}`);
            return true;
        }
        catch (error) {
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
        const width = customWidth || windowConfig_1.WINDOW_CONFIG.COACH.DEFAULT_WIDTH;
        const position = this.calculateCoachWindowPosition(width);
        return {
            width,
            height: windowConfig_1.WINDOW_CONFIG.COACH.DEFAULT_HEIGHT,
            x: windowConfig_1.WINDOW_CONFIG.COACH.OFFSET_X,
            y: windowConfig_1.WINDOW_CONFIG.COACH.OFFSET_Y,
            frame: windowConfig_1.WINDOW_CONFIG.COACH.FRAME,
            transparent: windowConfig_1.WINDOW_CONFIG.COACH.TRANSPARENT,
            alwaysOnTop: windowConfig_1.WINDOW_CONFIG.COACH.ALWAYS_ON_TOP,
            devTools: windowConfig_1.WINDOW_CONFIG.COACH.DEV_TOOLS,
            visibleOnAllWorkspaces: windowConfig_1.WINDOW_CONFIG.COACH.VISIBLE_ON_ALL_WORKSPACES,
            // Prevent user from maximizing or resizing the coach window
            resizable: windowConfig_1.WINDOW_CONFIG.COACH.RESIZABLE,
            maximizable: windowConfig_1.WINDOW_CONFIG.COACH.MAXIMIZABLE,
            fullscreenable: windowConfig_1.WINDOW_CONFIG.COACH.FULLSCREENABLE,
            minimizable: windowConfig_1.WINDOW_CONFIG.COACH.MINIMIZABLE,
            hasShadow: windowConfig_1.WINDOW_CONFIG.COACH.HAS_SHADOW
        };
    }
}
exports.WindowManager = WindowManager;
//# sourceMappingURL=windowManager.js.map