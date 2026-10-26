# macOS System-Audio Permission Matrix (Core Audio taps, M0)

Spec for SAYSO-513. Defines, for every permission case on every supported macOS
version, **what the native module must report and what the app must do**. It is
the input to M0 item 3 and our own final acceptance test.

- **Contractor:** run every row on a **signed** build and fill the **Observed**
  column. Any mismatch with an Expected column is a finding — report it, don't
  adjust the expectation.
- **Us:** re-run the same matrix on our signed release build before shipping the
  taps backend.

This document is a spec. The JS skeleton (provider changes, permissions step,
flag migration, stubbed native methods) is a separate follow-up issue built
from it. Current behavior it builds on: `docs/PERMISSIONS_FLOW.md`,
`docs/NATIVE_AUDIO_CONTRACT.md`.

---

## 1. Background

| | ScreenCaptureKit (today) | Core Audio process tap (new) |
|---|---|---|
| macOS | 13.0+ (`LSMinimumSystemVersion` is 13.0) | 14.4+ (tap API exists from 14.2, but the TCC prompt / `NSAudioCaptureUsageDescription` only from 14.4) |
| TCC service | `ScreenCapture` | `AudioCapture` (`kTCCServiceAudioCapture`) |
| Settings pane | Privacy & Security → Screen & System Audio Recording | Same pane, "System Audio Recording Only" section (to confirm per version) |
| Read status | `CGPreflightScreenCaptureAccess()` — public, non-prompting, reliable | **No public API.** Candidates: private `TCCAccessPreflight`, or a muted self-test tap |
| Prompt | `CGRequestScreenCaptureAccess()` | Reportedly fires on first `AudioDeviceStart` of a tap-backed aggregate device |
| Denied looks like | Start fails / preflight `false` | **Every call succeeds, buffers are silent** |
| Grant applies | Only after relaunch (bound at launch) | Unknown — M0 Q3 |

**Dev builds cannot test this.** Under `npm run dev` TCC attributes the request
to Terminal / the IDE, not Sayso. Every row must be run on a signed, packaged
build (`npm run package:staging` or a release artifact). Reset between runs with
`tccutil reset AudioCapture <bundle id>` — always scoped to the bundle id,
never a bare `tccutil reset AudioCapture`. The bundle id depends on the build:
`com.asksayso.app.staging` for `package:staging` (`build.staging.js`),
`com.asksayso.app` for production. Resetting the wrong one is a silent no-op
and invalidates the run.

### Backend selection

`getSystemAudioBackend()` decides which backend the **next** start uses:

- macOS < 14.4 → `screencapturekit`
- macOS ≥ 14.4 → `coreaudio_tap`, unless `setSystemAudioBackendOverride('screencapturekit')` is set
- The app may also pick SCK **for one session** as a permission fallback (§3, rule 2)

Everything below about the `AudioCapture` permission applies only when the
backend is `coreaudio_tap`. On `screencapturekit` the existing Screen Recording
flow applies unchanged.

---

## 2. Native contract assumed by this matrix

Additive to `docs/NATIVE_AUDIO_CONTRACT.md` (method names from the M0 brief;
M0 confirms or replaces them):

| Method | Returns | Must |
|---|---|---|
| `getSystemAudioBackend()` | `'coreaudio_tap' \| 'screencapturekit'` | Be sync and side-effect free |
| `checkSystemAudioPermission()` | `'granted' \| 'denied' \| 'not_determined' \| 'unknown'` | **Never prompt.** Return `unknown` rather than guess. Only called when backend is `coreaudio_tap` |
| `requestSystemAudioPermission()` | `Promise<same union>` | Show the prompt only when `not_determined`; resolve with the outcome after the user answers; never resolve `granted` on a guess |
| `setSystemAudioBackendOverride(v \| null)` | `void` | Force SCK; `null` clears |
| `startSystemAudioCapture()` on tap | rejection reasons below | Never start "healthy but silent" when the status is known missing |

Start-rejection reasons (start-promise channel, per the contract's two-channel rule):

- `tap_permission_denied` — status known `denied`
- `tap_permission_not_determined` — status known `not_determined`; native must **not** surface the prompt from a Cue start (prompts belong to the permissions step, not to the start of a sales call)
- `tap_start_timeout` — device never delivered buffers

Post-start lifecycle events (`setLifecycleEventCallback`):

- `tap_permission_revoked` — if native can detect a mid-session revoke (M0 Q5)
- `tap_silent_suspected secs=<n>` — tap delivered only exact zeros for `n` s (only if M0 shows a denied tap is distinguishable, Q6)

Never collapse these into a bare `false`.

---

## 3. Status policy (backend = `coreaudio_tap`)

Defined once; matrix rows reference these IDs.

### Onboarding (`/permissions` in the splash window)

The screen shows a **Microphone** row (unchanged) and a **System audio** row
(replaces the Screen Recording row on the tap backend).

| ID | Audio status | System audio row | Can Continue? |
|---|---|---|---|
| **P-G** | `granted` | Green | Yes (with mic granted) |
| **P-ND** | `not_determined` | "Allow System Audio" → `requestSystemAudioPermission()` → prompt | After the request resolves |
| **P-D** | `denied` | "Open System Settings" → audio pane. Screen polls `checkSystemAudioPermission()` every 1.5 s (like the Windows mic poll) and turns green on grant | Only once granted, or via "Quit and Reopen" if M0 says grants need a relaunch |
| **P-U** | `unknown` | Neutral "Can't verify — Sayso will check when your call starts", plus "Open System Settings" | Yes |

`relaunchOnComplete` on the tap backend is **false** if M0 confirms a grant
applies live (Q3), otherwise **true** (keep "Quit and Reopen").

### Cue start (`start-cue` guard)

Evaluated in order; first match wins.

| ID | Condition | Result |
|---|---|---|
| **S-1** | mic not granted | Unchanged: `permissions_denied`, splash → `/permissions` |
| **S-2** | audio `granted` | Start on **tap** |
| **S-3** | audio ≠ `granted` **and** Screen Recording granted (`CGPreflight`) | Start on **SCK for this session**. Log `backend_fallback reason=audio_<status>`. Non-blocking coach banner nudging the user to allow system audio |
| **S-4** | audio `unknown`, no Screen Recording | Start on **tap** with the silent-tap watchdog (§6) |
| **S-5** | audio `denied` / `not_determined`, no Screen Recording | `permissions_denied` (add `audio` field next to `mic`/`screen`), splash → `/permissions` |

The hard rule: **a session never starts healthy but silent when the permission
is known missing** (S-5 blocks; S-3 swaps to a backend that works). `unknown`
never blocks (S-3 / S-4): a user with nothing to click would be stuck, same
reasoning as the Windows mic `unknown` case.

### Completion flag

`permissions-complete` gains a version. Today it holds `1` (mic + Screen
Recording). The taps permissions step writes `2`.

- Backend `screencapturekit`: complete = flag ≥ 1 && mic && screen (unchanged)
- Backend `coreaudio_tap`: complete = flag ≥ 2 && mic && audio ∈ {`granted`, `unknown`}
- Self-heal (as today): mic && audio `granted` → write `2`, complete

A v1 flag on the tap backend is therefore incomplete → one-time re-permission
(case 7). No TCC reset is involved.

---

## 4. When the scoped `tccutil reset` runs

**Never on app update.** Existing users were never asked for `AudioCapture`, so
it is already `not_determined`; a reset gains nothing and would erase a user's
explicit deny. It runs only:

1. In the team-id (certificate) migration, `electron/utils/permissionsMigration.ts`,
   next to the existing `ScreenCapture` and `Microphone` resets — **only on macOS
   ≥ 14.4**, and its failure must not abort the other resets (on 13.x the
   service may be unknown to `tccutil` and exit non-zero; today any non-zero
   exit aborts the whole migration).
2. By QA / the contractor between matrix runs (case 6).

---

## 5. Matrix

Columns: **Native** = expected `checkSystemAudioPermission()` (and backend if
relevant) · **Request** = does the prompt show, does a grant apply without
relaunch · **App** = policy IDs from §3 · **Capture** = what the session does ·
**Observed** = contractor fills · **Notes**.

### Case 1 — Fresh install, never asked

| macOS | Native | Request | App | Capture | Observed | Notes |
|---|---|---|---|---|---|---|
| 14.4 | `not_determined`, backend `coreaudio_tap` | Prompt shows on request; grant applies live (expected) | P-ND; Cue before onboarding → S-1 (mic also not granted on a fresh install; S-5 only if mic was granted first) | No session until granted | | No flag file. Screen Recording never requested |
| 15 | `not_determined` | Same as 14.4 | P-ND; S-5 | No session until granted | | Note which pane / section the grant appears in |
| 26 | `not_determined` | Same as 14.4 | P-ND; S-5 | No session until granted | | Verify nothing changed in 26 TCC |

### Case 2 — User grants at the prompt

| macOS | Native | Request | App | Capture | Observed | Notes |
|---|---|---|---|---|---|---|
| 14.4 | Request resolves `granted`; next check `granted` | One prompt, no relaunch needed (expected) | P-G → Continue writes flag 2; S-2 | Tap delivers real audio on first session | | Record time from click to resolve |
| 15 | `granted` | Same | P-G; S-2 | Tap, real audio | | |
| 26 | `granted` | Same | P-G; S-2 | Tap, real audio | | |

### Case 3 — User denies at the prompt

| macOS | Native | Request | App | Capture | Observed | Notes |
|---|---|---|---|---|---|---|
| 14.4 | Request resolves `denied`; next check `denied` (or `unknown` if indistinguishable — Q1) | No second prompt on a second request; app opens Settings | P-D (or P-U); S-5 (or S-4) | `denied`: no session. `unknown`: tap session with the silent-tap watchdog (§6) — banner fires, never silent without a signal | | Key row for Q1 |
| 15 | `denied` | Same | P-D; S-5 | No session | | |
| 26 | `denied` | Same | P-D; S-5 | No session | | |

### Case 4 — Revoked in System Settings while Sayso runs

**4a mid-session** (tap session active) / **4b idle** (no session).

| macOS | Native | Request | App | Capture | Observed | Notes |
|---|---|---|---|---|---|---|
| 14.4 | 4a: `tap_permission_revoked` event if detectable; next check `denied`. 4b: next check `denied` | n/a | 4a: coach banner, session keeps running on mic side; next start S-5 (or S-3). 4b: next start S-5 / S-3 | 4a: prospect side ends **with a signal**, never silently. 4b: no tap start | | Does macOS kill / prompt "Quit & Reopen" like it does for Screen Recording? (Q5) |
| 15 | Same | n/a | Same | Same | | |
| 26 | Same | n/a | Same | Same | | |

### Case 5 — Re-granted in System Settings after a deny

| macOS | Native | Request | App | Capture | Observed | Notes |
|---|---|---|---|---|---|---|
| 14.4 | Check flips `denied` → `granted` without relaunch (expected) | User toggles in Settings; no prompt | P-D poll turns row green → Continue; S-2 | Tap, real audio, no relaunch | | If a relaunch is needed, `relaunchOnComplete` stays true (Q3) |
| 15 | Same | Same | Same | Same | | |
| 26 | Same | Same | Same | Same | | |

### Case 6 — `tccutil reset AudioCapture <bundle id>`

Run with Sayso quit and again with Sayso running.

| macOS | Native | Request | App | Capture | Observed | Notes |
|---|---|---|---|---|---|---|
| 14.4 | `not_determined` | Next request shows the prompt again | Next launch: flag 2 but audio `not_determined` → incomplete → P-ND. Start before that → S-3 / S-5 | No silent session | | If check reads `unknown` after reset, flag 2 counts as complete → S-3/S-4 path; record it |
| 15 | `not_determined` | Same | Same | Same | | |
| 26 | `not_determined` | Same | Same | Same | | Confirm the service name `AudioCapture` is accepted (Q7) |

### Case 7 — Existing user (Screen Recording granted, flag 1) updates to the taps build

| macOS | Native | Request | App | Capture | Observed | Notes |
|---|---|---|---|---|---|---|
| 14.4 | Backend `coreaudio_tap`; audio `not_determined` | Prompt on "Allow System Audio"; no relaunch (expected) | First launch: flag 1 → incomplete → `/permissions` with copy "Sayso now uses a lighter permission for call audio". User closes splash and starts Cue → **S-3** (SCK, still works) | Never blocked from a call; SCK until granted, tap after | | No `tccutil` reset. Screen Recording grant left in place |
| 15 | Same | Same | Same | Same | | |
| 26 | Same | Same | Same | Same | | |

### Case 8 — macOS 13.x and 14.0–14.3 (SCK)

| macOS | Native | Request | App | Capture | Observed | Notes |
|---|---|---|---|---|---|---|
| 13.x | Backend `screencapturekit`; `checkSystemAudioPermission()` not called | Existing SCK flow (preflight, `CGRequestScreenCaptureAccess`, relaunch) | Unchanged: flag 1 sufficient, mic + Screen Recording rows, "Quit and Reopen" | SCK, unchanged | | `NSAudioCaptureUsageDescription` must be inert |
| 14.0–14.3 | Backend `screencapturekit` | Same | Unchanged | SCK | | Tap API exists from 14.2 but must not be selected |

### Case 9 — macOS upgraded 14.3 → 14.4+ with the app already installed

| macOS | Native | Request | App | Capture | Observed | Notes |
|---|---|---|---|---|---|---|
| 14.4 | Backend flips to `coreaudio_tap`; audio `not_determined` | As case 7 | As case 7 (flag 1 → re-permission, S-3 meanwhile) | As case 7 | | Same app version — trigger is the OS, not an app update |
| 15 | Same | Same | Same | Same | | Common path: 14.x → 15 upgrade |
| 26 | Same | Same | Same | Same | | |

### Case 10 — Backend forced to SCK on 14.4+

| macOS | Native | Request | App | Capture | Observed | Notes |
|---|---|---|---|---|---|---|
| 14.4 | Backend `screencapturekit`; audio status ignored | Screen Recording flow | Case 8 behavior; flag 1 semantics. Clearing the override → §3 policy on next start | SCK | | Who sets the override (debug / settings / remote kill switch) is open |
| 15 | Same | Same | Same | SCK | | 15+ shows the periodic Screen Recording re-confirm prompt for SCK — record it |
| 26 | Same | Same | Same | SCK | | |

### Case 11 — Signing team changes (certificate migration) on the taps build

| macOS | Native | Request | App | Capture | Observed | Notes |
|---|---|---|---|---|---|---|
| 14.4 | After migration: `not_determined` | Prompt shows again | Migration resets ScreenCapture + Microphone + AudioCapture, deletes flag → P-ND | No session until re-granted (S-5; S-3 impossible, Screen Recording was reset too) | | Today's migration doesn't reset AudioCapture — follow-up |
| 15 | Same | Same | Same | Same | | |
| 26 | Same | Same | Same | Same | | |

### Case 12 — Mixed mic / audio states

| macOS | Native | Request | App | Capture | Observed | Notes |
|---|---|---|---|---|---|---|
| 14.4 | 12a mic denied + audio `granted`; 12b mic granted + audio `denied`, no Screen Recording | Each row requests independently | 12a: S-1, mic row only actionable. 12b: S-5, audio row only actionable | No session in either | | Rows never block each other's buttons |
| 15 | Same | Same | Same | Same | | |
| 26 | Same | Same | Same | Same | | |

---

## 6. `unknown` fallback

If M0 shows the status can't always be determined, the app degrades in this order:

1. **Screen Recording granted → SCK for the session (S-3).** Covers every existing user. Known-good, no silent session.
2. **Otherwise → tap + silent-tap watchdog (S-4).** If the prospect stream delivers only exact-zero samples for *N* s (placeholder 15 s; M0 Q6 sets it), main emits `tap_silent_suspected`, logs to Sentry, and the coach window shows a non-blocking banner ("Not hearing the other side? Allow System Audio Recording for Sayso") with an Open System Settings action. Once per session, cleared when audio arrives.
3. Onboarding shows P-U and lets the user continue.

Caveat for Q6: a granted tap with nothing playing may also deliver exact
zeros. If M0 can't tell the two apart, the banner copy stays conditional ("if
the other side is talking…") and the watchdog only arms after the mic has
detected speech.

---

## 7. Open questions for M0

Each lists the rows it unblocks.

1. **Can `denied` be told apart from `not_determined`** (and both from `granted`) without prompting? Compare private `TCCAccessPreflight(kTCCServiceAudioCapture)` vs a muted self-test tap. — Cases 1, 3, 6
2. **Does the self-test itself trigger the prompt** when `not_determined`? Latency? Is it safe at app start? — Cases 1, 6, policy P-*
3. **Does a grant apply to the running process without relaunch** (at the prompt and when toggled in Settings)? — Cases 2, 5, `relaunchOnComplete`
4. **When exactly does the prompt fire:** tap creation, aggregate-device creation, or `AudioDeviceStart`? — `requestSystemAudioPermission()` design
5. **Mid-session revoke:** zeros, an error, a device-dead callback, a process kill, or audio until restart? Detectable? — Case 4
6. **Is a denied tap distinguishable from a granted tap with nothing playing** (exact zeros vs noise floor, timestamps, flags)? — §6 watchdog
7. **`tccutil reset AudioCapture <bundleId>`:** correct service name on 14.4, 15 and 26? Exit code on 13.x? — Cases 6, 11
8. **Settings deep link** for the system-audio section (`x-apple.systempreferences:…`)? Does it differ by version? — P-D
9. **Does a Screen Recording grant imply AudioCapture** (or vice versa) on any version? — Cases 7, 9, S-3
10. **macOS 26 differences** in any of the above. — All 26 rows

---

## 8. Findings in the current app (handled in the follow-up unless noted)

- `NSAudioCaptureUsageDescription` (`package.json` `build.mac.extendInfo`) says "create multi-output devices for call recording" — wrong and alarming; it is the text of the system prompt. Proposed: "Sayso listens to your call's audio so it can transcribe the other side of the conversation and coach you in real time." Must land before the contractor validates the prompt in M0.
- `MacPermissionsProvider.requirements` is a static `{ screen: true, relaunchOnComplete: true }`; on the tap backend it must come from the backend (and `screen` becomes "system audio").
- `permissionsMigration.ts` resets only `ScreenCapture` and `Microphone`, and aborts on any non-zero exit (§4).
- `start-cue`'s `permissions_denied` result carries `mic` / `screen` only; needs `audio` and the chosen backend.
- `permissions-complete` is unversioned (§3 completion flag).

---

## 9. Acceptance procedure (our signed build)

1. Build signed + notarized (`npm run package:staging`), install to `/Applications`.
2. For each case × version: reset with the scoped `tccutil` against the staging bundle id `com.asksayso.app.staging` (`com.asksayso.app` for a production artifact) (and delete `~/Library/Application Support/sayso-app-staging/permissions-complete` (`sayso-app` for production) where the case starts from fresh), set up the precondition, run, record Observed.
3. For every row that starts a session: play known audio (a Zoom / Meet test call) and confirm prospect transcripts arrive; for every blocked row confirm no session was created server-side.
4. Any Observed ≠ Expected → ticket, linked here.
