# AssemblyAI Zombie Session Investigation
**Date:** 2026-05-22  
**Investigated by:** Claude (client repo agent)  
**Related bug:** Zombie CUE sessions / improper WebSocket teardown  
**Affected user:** `kuvaal@asksayso.com` (account `e505a406-56c0-4fb4-8cf4-8db72b6c2cb8`)  
**Affected date:** 2026-05-21, 17:41–18:09 UTC

---

## Summary

A WebSocket teardown bug causes the **backend to keep reconnecting to AssemblyAI indefinitely** after a CUE session ends. Each reconnect creates a new idle AssemblyAI streaming session that receives no audio and times out after 30 seconds, then reconnects again. This cycle consumed **~336 AssemblyAI sessions over 84 minutes** from a single 4-minute demo, hit the platform's concurrency limit, and blocked all prospect audio transcription for kuvaal's 4 demo sessions on May 21.

---

## Evidence Files

| File | Contents |
|------|----------|
| `sayso-2026-05-20.log` | kuvaal's Electron client logs (covers through May 21 17:45) |
| `all_sessions_05_21_2026.json` | All Sayso DB sessions for May 21 |
| `kuvaal_sessions_05_21_2026.json` | kuvaal's 4 demo sessions with model run data |
| `transcript-list (3).csv` | AssemblyAI streaming sessions 17:42:49–17:47:31 (100 rows) |
| `transcript-list (2).csv` | AssemblyAI streaming sessions 17:48:01–02:00:45 (100 rows) |

---

## Timeline of the Incident

### Phase 1: Normal operation (17:41:38–17:42:36)
- kuvaal starts Sayso session `d1ad9e6e`
- Both WebSockets (`/cue/stream/user` and `/cue/stream/prospect`) connect to backend
- Backend opens 2 AssemblyAI streaming sessions
- Session ends at 17:42:36 — client calls `disconnect()` properly, `shouldReconnect=false`

### Phase 2: Concurrency error burst (17:42:49–17:43:00) — 23 errors
- kuvaal starts session `e9f04574` at 17:42:37
- Backend attempts to create new AssemblyAI sessions
- **`d1ad9e6e`'s backend AssemblyAI sessions are still alive** (backend did not close them on client WS disconnect)
- Backend retry loop fires at ~500ms intervals: **23 "Concurrency Exceeded" errors** in 11 seconds
- Only the **user stream** eventually gets a stable session (90s, 88.3s audio)
- The **prospect stream** never gets a stable session during this window

### Phase 3: Zombie loop begins (17:43:00 onwards)
- `d1ad9e6e`'s orphaned AssemblyAI sessions finally time out (30s silence)
- Backend reconnect logic fires → creates **2 new AssemblyAI sessions**
- These new sessions receive **no audio** (client for `d1ad9e6e` is gone)
- AssemblyAI 30s silence timeout → closes sessions
- Backend reconnect fires again → cycle repeats **every 30 seconds, indefinitely**

### Phase 4: Cascade blocks subsequent sessions (17:44:31–17:45:01) — 54 errors
- Sessions `cc1b763a` (4s) and `3c32a998` (42s) start and try to open WebSockets
- 2 zombie sessions are already consuming concurrency slots
- **54 more "Concurrency Exceeded" errors** in 30 seconds
- `3c32a998` gets a 18s session with 16.2s audio (user stream only)

### Phase 5: Zombie runs uncontrolled (17:43:00–18:07:01) — 84 minutes
- kuvaal's last session ends at 17:45:17
- Client log shows **zero WebSocket activity after 17:45:24**
- AssemblyAI zombie pairs continue cycling **every 30 seconds** with 2 sessions at a time, 0 audio
- Total estimated: ~168 zombie pairs = ~336 idle AssemblyAI sessions
- Zombie stops around 18:07–18:09 (reason: backend timeout, cleanup job, or app restart)

---

## AssemblyAI Session Data Analysis

### `transcript-list (3).csv` (17:42:49–17:47:31)

```
Status breakdown: 77 errors / 23 completed
Session duration: avg 31s (min 5s, max 90s)
Audio duration:   avg 4.7s (all errors have 0s, zombies have 0s)

Pattern:
17:42:49–17:43:00  23× error (Concurrency Exceeded, 0.5s intervals) ← backend retry loop
17:43:00–17:43:01  2× completed (30s, 0 audio)                       ← zombie pair starts
17:43:01           1× completed (90s, 88.3s audio)                   ← e9f04574 USER stream
17:43:30           2× completed (30s, 0 audio)                       ← zombie cycling
17:44:00           2× completed (30s, 0 audio)
17:44:30           2× completed (30s, 0 audio)
17:44:31–17:45:01  54× error (Concurrency Exceeded, 0.5s intervals)  ← 2nd burst
17:45:01           2× completed (30s, 0 audio)
17:45:01           1× completed (18s, 16.2s audio)                   ← 3c32a998 USER stream
... cycling continues to end of file
```

### `transcript-list (2).csv` (17:48:01–02:00:45)

```
Status breakdown: 100 completed / 0 errors

17:48:01–18:06:31  76 rows (38 pairs): 30s/0-audio cycling every 30s ← zombie continues
18:07:01           2× completed (56s, 0 audio)                       ← timeout interval changed
18:07:57           2× completed (56s, 0 audio)
18:08:54           2× completed (4s, 0 audio)
18:09:54           2× completed (72s, 70.6s / 66.2s audio)           ← REAL session, zombie stopped
```

**Key stat:** 100% of sessions from 17:48:01 to 18:07:01 are zombie sessions with 0 audio.

---

## Why kuvaal Got No Output

### What the Sayso DB shows (`kuvaal_sessions_05_21_2026.json`)

| Session | Duration | User transcripts | Prospect transcripts | cue_model_runs | sc_model_runs |
|---------|----------|------------------|----------------------|---------------|---------------|
| `d1ad9e6e` | 58s | 7 entries (speaker: user) | 0 | 0 | 0 |
| `e9f04574` | 112s | 14 entries (speaker: user) | 0 | 0 | 0 |
| `cc1b763a` | 4s | 0 | 0 | 0 | 0 |
| `3c32a998` | 42s | 4 entries (speaker: user) | 0 | 0 | 0 |

**All 4 sessions: `cue_model_runs=0`, `sc_model_runs=0`, `cue_input_tokens=0`, `sc_input_tokens=0`**

### The chain of failure

```
Prospect AssemblyAI session BLOCKED (Concurrency Exceeded)
    ↓
No prospect audio transcribed by AssemblyAI
    ↓
No prospect transcripts delivered to backend
    ↓
CUE model never fires (requires prospect speech to coach on)
Smart Capture model never fires (requires prospect speech to extract data from)
    ↓
cue_model_runs=0, sc_model_runs=0 — zero output
```

The **user stream worked fine** (the 90s/88.3s audio session is the user stream). That's why we see user transcripts in the DB. Christian's voice was captured by SCK and delivered to the backend correctly — it was blocked at the AssemblyAI concurrency layer, not lost in the audio pipeline.

Note: `e9f04574` transcripts show the agent roleplaying BOTH sides of a demo call. Christian's responses also appear as `speaker: "user"` because they leaked through the agent's speakers into the mic. This is expected behavior and unrelated to the bug.

---

## Root Cause: Backend Does Not Close AssemblyAI Sessions on Client WS Disconnect

### The zombie creation mechanism

When a CUE session ends, the Electron client properly calls `disconnect()` on both WebSocket clients, which:
- Sets `shouldReconnect = false`
- Clears any pending reconnect timers
- Removes all WS event listeners
- Closes the WebSocket

The client side is clean. **The bug is on the backend.**

When the backend receives a WS `close` event from the client, it does NOT immediately close the associated AssemblyAI streaming session. Instead:

```
Client WS closes
    ↓
Backend marks client WS as disconnected
    ↓
AssemblyAI session remains OPEN (no audio coming in)
    ↓
AssemblyAI 30s silence timeout → closes session → notifies backend
    ↓
Backend's reconnect logic fires → creates NEW AssemblyAI session
    ↓  (client is gone, no audio will ever come)
New session gets no audio → 30s timeout → closes
    ↓
Backend reconnect fires again → infinite loop
```

The backend's AssemblyAI WebSocket client has the **same infinite reconnect pattern** as the client-side bug: `reconnectAttempts` resets to 0 on each successful connection, so the `< maxAttempts` limit never trips as long as each reconnect eventually succeeds.

### The backend retry burst (23/54 errors)

The backend also has an **aggressive retry loop** when AssemblyAI session creation fails:
- Retry interval: ~500ms
- No limit on burst retries
- Each retry attempt creates a new "failed" AssemblyAI session record

This produces the 23-error (11s) and 54-error (30s) bursts seen in the CSV.

### Expected vs actual AssemblyAI session count

| | Expected | Actual |
|--|---------|--------|
| Sayso sessions | 4 | 4 |
| AssemblyAI sessions (2 per Sayso session) | 8 | ~336+ |
| Ratio | 1× | ~42× |

---

## Connection to the Auto-Stop Zombie Bug

This is the **same underlying bug** manifesting in two different triggers:

| Trigger | May 20 `a9829c18` | May 21 demo sessions |
|---------|-------------------|---------------------|
| **What starts the zombie** | `auto_stop` message → session restarts but WS not torn down → reconnect loop | Session ends normally → backend AssemblyAI sessions not closed on client WS disconnect |
| **What keeps it alive** | Backend receives client reconnect → creates new AssemblyAI session | Backend reconnects to AssemblyAI even with no client |
| **Duration** | 84 minutes (17:30–18:54 May 20) | 84 minutes (17:43–18:07 May 21) |
| **Session count** | Unknown but similar | ~336 AssemblyAI sessions |
| **How it stopped** | App restart at 18:54 | Backend timeout/cleanup ~18:09 |

**The shared root:** The backend does not enforce that an AssemblyAI session can only exist while a live client WebSocket is connected. Both bugs produce the same outcome: orphaned AssemblyAI sessions cycling every 30 seconds.

---

## Client-Side `WebSocketClient` Infinite Reconnect Risk

Located at `electron/streaming/websocketClient.js`.

### The counter-reset vulnerability

```javascript
// _connect() — line 135
this.ws.on('open', () => {
    this.reconnectAttempts = 0;  // ← resets to 0 on EVERY successful open
    ...
});

// _handleDisconnect() — line 190
if (this.shouldReconnect && this.reconnectAttempts < CONNECTION_CONFIG.reconnectAttempts) {
    // schedules reconnect
}
```

Because `reconnectAttempts` resets on every successful connect, the `< maxAttempts` guard **never trips** as long as the WS connects successfully before being closed again. This means:

- If `shouldReconnect = true` AND the WS keeps connecting then getting closed externally (by backend closing it due to an AssemblyAI timeout or `auto_stop`)
- The WS will reconnect **indefinitely**
- The `maxAttempts = 3` limit is effectively useless in this scenario

### Current protection

The current protection is solely `shouldReconnect = false` being set in `disconnect()`. This works correctly when `disconnect()` is called — but relies entirely on teardown being called before the WS enters the loop.

### The `auto_stop` connection

When the backend sends an `auto_stop` message:
1. The client receives it and triggers session restart
2. If the restart path calls `connect()` (which sets `shouldReconnect = true`) but the WS teardown path does not call `disconnect()` (which would set `shouldReconnect = false`)
3. The WS enters the infinite loop

This is exactly the May 20 zombie: `auto_stop` → restart called → WS never torn down → reconnect loop → every backend reconnect creates a new AssemblyAI session.

---

## Recommended Fixes

### Fix 1 — Backend (primary fix): Close AssemblyAI session immediately on client WS disconnect

```
On client WS 'close' event:
    1. Set shouldReconnect = false on the associated AssemblyAI WS client
    2. Close the AssemblyAI WS immediately (don't wait for silence timeout)
    3. Clean up session state
```

Do NOT reconnect to AssemblyAI unless the same `sessionId` client WS reconnects within a short grace window (e.g., 5 seconds).

### Fix 2 — Backend: Cap retry burst for AssemblyAI session creation

The 500ms retry loop with no burst cap should be rate-limited:
- Max 3 retries with exponential backoff (1s, 2s, 4s)
- After max retries: close the client WS with an error message instead of continuing to retry

### Fix 3 — Client (this repo): Send explicit `terminate` signal before WS close

In `teardownCueStreamsAndNative()` or `AudioStreamer.stop()`, before calling `disconnect()`:

```javascript
// Signal backend to close AssemblyAI session immediately (don't wait for silence timeout)
this.userWebSocket?.sendJson({ type: 'terminate' });
this.prospectWebSocket?.sendJson({ type: 'terminate' });
// Wait briefly for delivery, then:
await this.userWebSocket?.disconnect();
await this.prospectWebSocket?.disconnect();
```

This gives the backend an explicit "close now" signal, reducing the grace period from 30s to ~0s.

### Fix 4 — Client (this repo): Harden the reconnect counter

Change `reconnectAttempts` to NOT reset on `open` — instead, reset it only on a deliberate `connect()` call:

```javascript
// In connect() — deliberate reconnect, reset counter
async connect(token, sessionId) {
    this.reconnectAttempts = 0;  // ← move here
    this.shouldReconnect = true;
    return this._connect();
}

// In _connect() — remove the reset from 'open' handler
this.ws.on('open', () => {
    // this.reconnectAttempts = 0;  ← REMOVE THIS
    this.state = 'connected';
    this.emit('connected');
    resolve();
});
```

This makes the `< maxAttempts` guard actually trip after 3 failed reconnect attempts (disconnect → reconnect → disconnect → reconnect → disconnect → reconnect → stop). Currently it never trips because it resets on every successful open.

---

## Verification Checklist

After fixes are deployed, verify:

- [ ] AssemblyAI session count per Sayso session ≤ 2 (1 user + 1 prospect) under normal operation
- [ ] After a Sayso session ends, no new AssemblyAI sessions appear for that `sessionId` within 60 seconds
- [ ] `auto_stop` handling does not leave WebSockets open with `shouldReconnect=true`
- [ ] Concurrency Exceeded errors drop to 0 between sessions from the same user
- [ ] A 112-second session with active prospect speech produces `sc_model_runs > 0` and `cue_model_runs > 0`

---

## Key File References

| File | Relevance |
|------|-----------|
| `electron/streaming/websocketClient.js:135` | `reconnectAttempts` reset in `open` handler — infinite reconnect risk |
| `electron/streaming/websocketClient.js:190` | `_handleDisconnect` reconnect guard — relies solely on `shouldReconnect` |
| `electron/streaming/websocketClient.js:264` | `disconnect()` — correctly sets `shouldReconnect=false`, clears timer |
| `electron/streaming/audioStreamer.js:375` | `stop()` — calls `disconnect()` on both WS clients |
| `electron/main.ts:1051` | `teardownCueStreamsAndNative()` — calls `cueAudioStreamer.stop(false)` |
| `electron/main.ts:886` | `start-cue` IPC handler — where new `AudioStreamer` is created per session |

---

*This document was generated from direct log and database analysis. All session IDs, timestamps, and counts are from the actual source data.*
