// Window configuration constants
const WINDOW_CONFIG = {
  COACH: {
    DEFAULT_WIDTH: 285,
    DEFAULT_HEIGHT: 45,
    MIN_WIDTH: 285,
    MAX_WIDTH: 1200,
    MIN_HEIGHT: 45,
    MAX_HEIGHT: 1400,
    OFFSET_Y: 80, 
    OFFSET_X: 100, 
    FRAME: false,
    TRANSPARENT: true,
    ALWAYS_ON_TOP: true,
    RESIZABLE: false,
    MAXIMIZABLE: false,
    MINIMIZABLE: false,
    FULLSCREENABLE: false,
    BORDERLESS: true,
    HAS_SHADOW: false
  },
  DASHBOARD: {
    DEFAULT_WIDTH: 1024,
    DEFAULT_HEIGHT: 768,
    MIN_WIDTH: 800,
    MIN_HEIGHT: 600
  },
  PLAYBOOK: {
    WIDTH: 520,
    HEIGHT: 500,
    MIN_WIDTH: 360,
    MIN_HEIGHT: 300,
    GAP_X: 20,
    FRAME: false,
    TRANSPARENT: true,
    ALWAYS_ON_TOP: true,
    RESIZABLE: true,
    MAXIMIZABLE: false,
    MINIMIZABLE: false,
    FULLSCREENABLE: false,
    HAS_SHADOW: false
  },
  // SAYSO-530 design. The window is the card plus transparent room for its
  // shadow; the insets place the card (not the window) as in the Figma mock.
  CALL_PROMPT: {
    CARD_WIDTH: 344,
    CARD_HEIGHT: 120,
    SHADOW_PADDING: 16,
    INSET_RIGHT: 31,
    INSET_TOP: 52
  }
};

export { WINDOW_CONFIG };