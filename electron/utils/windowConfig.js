// Window configuration constants
const WINDOW_CONFIG = {
  COACH: {
    DEFAULT_WIDTH: 700,
    DEFAULT_HEIGHT: 48,
    MIN_WIDTH: 300,
    MAX_WIDTH: 1200,
    MIN_HEIGHT: 48,
    MAX_HEIGHT: 600,
    OFFSET_Y: 40, // Distance from top of screen
    OFFSET_X: 20, // Distance from left of screen
    FRAME: false,
    TRANSPARENT: true,
    ALWAYS_ON_TOP: true,
    DEV_TOOLS: false,
    VISIBLE_ON_ALL_WORKSPACES: true,
    // Prevent user from maximizing or resizing the coach window
    // RESIZABLE: false,
    // MAXIMIZABLE: false,
    RESIZABLE: true,
    MAXIMIZABLE: true,
    FULLSCREENABLE: false,
    MINIMIZABLE: true // Allow minimizing but not maximizing
  },
  DASHBOARD: {
    DEFAULT_WIDTH: 1024,
    DEFAULT_HEIGHT: 768,
    MIN_WIDTH: 800,
    MIN_HEIGHT: 600
  }
};

module.exports = { WINDOW_CONFIG };