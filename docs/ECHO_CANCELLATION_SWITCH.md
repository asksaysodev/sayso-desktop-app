# Echo cancellation: turning it on or off (Windows)

Echo cancellation removes the prospect's voice, played on the rep's speakers,
from the rep's microphone before the mic audio reaches the app. It lives in
the native audio module (`electron/native-audio`), and the JavaScript pipeline
stays as it is.

**It ships off.** With it off, the mic reaches the app exactly as before:
same bytes, same chunks, same format.

You can turn it on in any of these ways. Pick one, or combine them.

| Way | Changes | Who decides | When it applies |
|---|---|---|---|
| A. On by default in the native module | one value in `binding.gyp` | the build | every session, no app change |
| B. Switch it on from the app | one call in Electron | the app | from the next mic packet, also mid-call |
| C. A setting in the app | B, driven by a stored preference or a remote flag | the user or you | from the next mic packet |
| Testing only | an environment variable | a tester | when the app starts |

The full method contract is in
[NATIVE_AUDIO_CONTRACT.md](NATIVE_AUDIO_CONTRACT.md)
(`setEchoCancellationEnabled`, `getEchoCancellationState`, `aec_*` events).

---

## A. On by default in the native module

In `electron/native-audio/binding.gyp`, inside the `OS=='win'` block:

```
"defines": [ "SAYSO_AEC_DEFAULT_ON=0" ],
```

Change `0` to `1`, then rebuild the module (`npm run rebuild-native-win`, or
your normal packaging build). Every session then starts with echo cancellation
on, and the app needs no changes.

The app can still switch it off at runtime with
`setEchoCancellationEnabled(false)`.

## B. Switch it on from the app

The module's JavaScript wrapper (`electron/native-audio/index.js`) already
exposes the switch:

```js
nativeAudio.setEchoCancellationEnabled(true);   // returns false when the build has no echo cancellation
nativeAudio.getEchoCancellationState();         // { built, enabled, active, delayMs?, lastError? } or null
```

Both are safe on every platform. On macOS, and on builds without echo
cancellation, the first returns `false` and the second returns `null`, and
nothing else happens. Both are declared as optional methods in
`electron/audio/IAudioProvider.ts` (`EchoCancellationState` is the state's type).

**Call it before the mic starts.** In `electron/recorder.ts`, in
`startUserStreaming`, just before
`nativeAudio.startMicrophoneCapture({ streamingCallback })`:

```ts
    nativeAudio.setEchoCancellationEnabled?.(true);
```

One call is enough: the switch also stays set for later sessions. Calling it
before every mic start is harmless, and keeps the app in control if the value
comes from a setting.

## C. A setting in the app

Use B with a value instead of `true`:

```ts
    nativeAudio.setEchoCancellationEnabled?.(echoCancellationPreference);
```

The value can come from wherever you keep preferences: a user toggle in
settings, a per-account or remote feature flag, or "on for testers first".

The switch is **live**. Calling it during a call takes effect from the next
mic packet, with no mic audio lost, repeated or reordered. A settings toggle
can call it directly while a session runs. No restart is needed.

## Testing only: environment variable

For a tester's machine, without rebuilding or changing code, set this before
starting the app:

```powershell
$env:SAYSO_AEC_ENABLE="1"   # on at launch
$env:SAYSO_AEC_ENABLE="0"   # off at launch
```

It overrides the `binding.gyp` default when the app starts. A later
`setEchoCancellationEnabled` call from the app still wins.

---

## Checking that it works

Every native lifecycle event is already written to the app log as
`[NativeAudio] lifecycle: <event>` (see `initAudioProvider` in
`electron/audio/audioManager.ts`). With echo cancellation on, look for:

| Log line | Meaning |
|---|---|
| `aec_switched on` | the app called the switch (not shown for way A or the environment variable) |
| `aec_started` | echo cancellation started with the mic |
| `aec_engine_ready engine=webrtc_aec3 reason=…` | the canceller is running |
| `aec_stats … mode=aec … reduction_db=…` | every 60 s: how much echo was removed, the added latency, the cost |
| `aec_stopped …` | session totals when the mic stops |

`getEchoCancellationState()` returns `active: true` while cleaned audio is
being delivered. It returns `active: false` while the mic is passed through
unchanged, for example before the prospect's audio has started, or when no
call audio is playing.

A simple check on speakers: during a call, mute yourself in the meeting app,
stay silent and let the other person talk. Your mic should produce no
transcript, while the prospect's side is transcribed as normal.

## Failures and monitoring

If echo cancellation cannot run, the mic passes through exactly as with it
off. It never blocks or drops the rep's audio.

A mid-call failure stays silent for the rep: the mic keeps working, and there
is nothing for them to do. It is logged, and reported as below. The code is in
`electron/audio/audioManager.ts` and `electron/audio/aecCallSummary.ts`.

**Sentry warnings, deduplicated.** A failure is emitted as
`aec_failed reason=…` and sent to Sentry as a warning
(`[native-audio] aec_failed …`), at most **once per failure type per call**
(event + reason; a repeat with another error code counts as the same type).
Nothing is sent per packet or per chunk.

**One summary per call.** When a call ends, the app logs one line:

```
[Cue] Echo cancellation summary — session=<id> enabled=true active=true active_pct=93.4 delay_ms=12 added_latency_ms=21.5 reduction_db=18.0 engine=webrtc_aec3 fallbacks=aec_reference_missing:system_audio_not_startedx1 last_error=none
```

| Field | Meaning |
|---|---|
| `enabled` / `active` | Switched on during the call / delivered cleaned audio during the call |
| `active_pct` | Share of the rep's mic audio that was echo-cancelled |
| `delay_ms` | The engine's echo delay estimate (speaker → mic) |
| `added_latency_ms` / `reduction_db` | Mic latency added, and echo removed, over the call's last mic session |
| `fallbacks` | Each time the mic went out without cancellation, by event and reason, with a count |
| `last_error` | The call's last failure, if any |

`aec_reference_missing:system_audio_not_started` is normal: the call starts
before the prospect's audio does.

In Sentry, the same summary sits on the scope for the whole call, so **any
event during the call** carries it: context `echo_cancellation` and tags
`aec_enabled`, `aec_active`, `aec_fallback` (`none`, `no_reference` or
`problem`). This sends nothing by itself. One Sentry event,
`[Cue] Echo cancellation fell back during the call` (warning), is sent at the
end of a call **only** when echo cancellation was on and fell back for a real
problem: a failure, a build without the library, or audio that skipped
cancellation (`aec_backlog_dropped`, `aec_queue_full`). Calls with echo
cancellation off, and calls that went normally, send nothing.

The 60-second `aec_stats` lines stay in the app log only.

**Mic diagnostics.** The existing mic diagnostics
(`electron/audio/micDiagnostics.ts`, attached to mic start-failure, stall and
teardown reports) include echo cancellation:

- tags `aec` (`off`, `active`, `passthrough`, or `unavailable` on macOS) and
  `aec_built`;
- extra `mic_aec`: `{ built, enabled, active, delayMs, lastError }`;
- the log line: `aec=… aec_built=… aec_delay_ms=… aec_last_error=…`.

## Requirements

- **Build library.** Windows builds need the WebRTC echo library at
  `electron/native-audio/deps/webrtc_apm/win-x64/sayso_webrtc_apm.lib`. It is
  committed with Git LFS: run `git lfs install` once, then `git lfs pull`.
  Packaging builds stop with an error if it is missing. Details and how to
  rebuild it: [WEBRTC_LIBRARY_BUILD.md](WEBRTC_LIBRARY_BUILD.md).
- **Development builds without the library.** They log
  `aec_engine_unavailable reason=not_built` and pass the mic through.
- **Headsets.** Echo cancellation is only needed on speakers. With a headset
  there is no echo, and leaving it on does no harm.
- **Laptop mic effects.** Some laptops have their own microphone noise
  cancellation (for example HP Audio Control), which mutes the mic while the
  speakers play, before any app receives the audio. That affects the app with
  or without echo cancellation. Turning the laptop's feature off gives the
  complete rep audio.
