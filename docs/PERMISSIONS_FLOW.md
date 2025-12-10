# Permissions Flow Documentation

## Overview

Sayso AI Coach requires **microphone** and **screen recording** permissions to function. Following industry best practices (Discord, Zoom, Slack), we implement **just-in-time permissions** - only requesting access when the user actually tries to use the feature.

Permissions are requested when the user attempts to open the Coach Window, not at app startup or login.

---

## Required Permissions

### 1. **Microphone Access**
- **Why**: Capture user's voice during calls
- **macOS Setting**: System Settings > Privacy & Security > Microphone
- **Status Check**: ✅ Reliable via `systemPreferences.getMediaAccessStatus('microphone')`

### 2. **Screen & System Audio Recording**
- **Why**: Capture prospect's voice (system audio) during calls
- **macOS Setting**: System Settings > Privacy & Security > Screen & System Audio Recording
- **Status Check**: ❌ Cannot reliably check without triggering request (macOS limitation)

---

## Entry Points

The Coach Window can be opened from three places:

**Button** `src/components/CoachCTA.jsx` | User clicks "Start AI Coach" |
**Keyboard shortcut** `electron/main.cjs` | User presses `Control+S` |
**Tray Menu** `src/trayMenu/TrayMenuApp.jsx` | User clicks tray menu item |

All entry points call `openCoachWindow()` from the store, ensuring **single source of truth** for permission handling.

---

## Flow Diagram

```
User Action (Button/Shortcut/Tray)
    ↓
openCoachWindow() in Store
    ↓
Check localStorage: 'coachPermissionsGranted'
    ↓
    ├─→ true? → Open Coach Window ✅
    │
    └─→ false? → Show Sayso Modal
                     ↓
                User clicks "Allow"
                     ↓
                Request Native Permissions
                     ↓
                macOS Dialogs (Mic → Screen)
                     ↓
                     ├─→ Both Granted? → Save to localStorage
                     │                → Close Modal
                     │                → Open Coach Window ✅
                     │
                     └─→ Denied? → Update Modal Text
                                 → "Go to System Settings"
                                 → User must manually enable
```

---

## User Experience

### First Time Using Coach

1. User clicks "Start AI Coach"
2. Sayso modal appears
3. User clicks "Allow"
4. macOS shows microphone dialog → User clicks "OK"
5. macOS shows screen recording dialog → User clicks "Allow"
6. Coach window opens
7. `localStorage` is set to `'true'`

### Subsequent Uses

1. User clicks "Start AI Coach"
2. Coach window opens immediately (no modals)

### If User Denies

1. User clicks "Start AI Coach"
2. Sayso modal appears
3. User clicks "Allow"
4. macOS dialog appears → User clicks "Don't Allow"
5. Sayso modal updates: "Go to System Settings"
6. User must manually enable in System Settings

---

## Summary
- **Just-in-time**: Permissions requested when user opens coach, not at startup
- **Single source of truth**: All entry points go through `openCoachWindow()` in store
- **Two-stage modal**: Our explanation modal → Native macOS dialogs
- **localStorage tracking**: Since we can't reliably check screen recording permission
- **System Settings fallback**: If denied, user must manually enable
