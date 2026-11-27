// How a Cue session was started (SAYSO-696), sent with POST /cue/session/new and stored in
// coach_sessions.launch_source. Keep in step with the server's utils/cueLaunchSource.js.
//
// A CoachOpener is how the coach widget was opened; it holds for every session started in that
// window until it closes. 'reset' is the coach's Reset button, which replaces a running session.
export type CoachOpener = 'shortcut' | 'tray' | 'web_app' | 'call_prompt' | 'other';
export type CueLaunchSource = CoachOpener | 'reset';
