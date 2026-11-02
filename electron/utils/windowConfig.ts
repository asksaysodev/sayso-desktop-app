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
    OFFSET_X: 40, 
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
  // SAYSO-566: the coach window in its prompt state. WIDTH must match
  // --pill-width in src/coachWindow/styles/CoachPrompt.css; HEIGHT is the
  // coach's own resting window height (WINDOW_HEIGHT_SIZES.BASE).
  CALL_PROMPT: {
    WIDTH: 354,
    HEIGHT: 54
  }
};

export { WINDOW_CONFIG };