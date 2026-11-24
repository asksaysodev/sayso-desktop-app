# WebRTC echo-cancellation library (Windows)

On Windows, echo cancellation uses the WebRTC AEC3 echo canceller. It is built
once, from WebRTC source outside this repo, as a single static library,
`sayso_webrtc_apm.lib`. The built library is committed to this repo with
**Git LFS**. `native_audio.node` links it in, so no extra DLL ships with the app.

This guide covers:

1. how the pieces fit together;
2. the committed library (Git LFS) and CI;
3. rebuilding it yourself, and committing the new one;
4. checking that echo cancellation works in the built app;
5. troubleshooting.

To turn echo cancellation on or off, see
[ECHO_CANCELLATION_SWITCH.md](ECHO_CANCELLATION_SWITCH.md).

## How it fits together

```
WebRTC source (webrtc.googlesource.com, revision d943abd3)
  + our wrapper: electron/native-audio/third_party/sayso_webrtc_apm/
        │   built by electron/native-audio/scripts/build-webrtc-apm.ps1
        ▼
electron/native-audio/deps/webrtc_apm/win-x64/sayso_webrtc_apm.lib
        │   linked by electron/native-audio/binding.gyp (npm run rebuild-native-win)
        ▼
electron/native-audio/build/Release/native_audio.node   (ships in the installer)
```

| Piece | What it is |
|---|---|
| `third_party/sayso_webrtc_apm/` | Our wrapper: a GN target (`BUILD.gn`) and a small C interface over AEC3 (`sayso_apm.h`, `sayso_apm.cc`). Only the echo canceller is built: no noise suppression or gain control. |
| `scripts/build-webrtc-apm.ps1` | Copies the wrapper into a WebRTC checkout, builds the library, copies it to `deps/webrtc_apm/win-x64/`, and regenerates `THIRD_PARTY_NOTICES.txt`. |
| `scripts/webrtc-apm-status.js` | Run by `binding.gyp`. It tells the build whether the library is present, and says what to do when the file is only a Git LFS pointer. |
| `.gitattributes` (repo root) | Stores the library in Git LFS. `.gitignore` keeps the rest of `deps/` out of git. |
| `scripts/native-build-env.js` (repo root) | Used by the packaging builds. On Windows it sets `SAYSO_REQUIRE_WEBRTC_APM=1`, so packaging **stops with an error** if the library is missing, rather than shipping without echo cancellation. |
| `THIRD_PARTY_NOTICES.txt` | Licences of everything inside the library (all permissive: BSD, Apache 2.0, MIT-style). It ships next to `native_audio.node`. |

The library is needed only at build time. It is linked into
`native_audio.node`, and `package.json` keeps it, the wrapper and the scripts
out of the installer.

All paths below are relative to `electron/native-audio/` unless they start
with the repo root.

## The committed library (Git LFS)

The library is committed at
`electron/native-audio/deps/webrtc_apm/win-x64/sayso_webrtc_apm.lib`, stored
with Git LFS. It was built with the same toolchain as the release workflow:

| | |
|---|---|
| File | `sayso_webrtc_apm.lib`, 10,602,312 bytes |
| SHA-256 | `788C52708E48823E5FC00A168ADD940D1D0B0A4EFA2D34DE817D929607ABB2B1` |
| WebRTC revision | `d943abd3750afb3fb6c956a4bafc4ef294b55198` |
| Toolchain | Visual Studio 2022 Build Tools 17.14 (MSVC 14.44, v143), Windows SDK 10.0.26100, x64 Release, static CRT (`/MT`) |

1. **Install Git LFS once per machine** (it comes with Git for Windows) and
   fetch the library, from the repo root:
   ```powershell
   git lfs install
   git lfs pull
   ```
   A clone made after `git lfs install` fetches it automatically. Without Git
   LFS the file is a small text pointer; the native build then says so and
   prints these two commands.
2. Optional: check the hash (PowerShell):
   ```powershell
   Get-FileHash electron\native-audio\deps\webrtc_apm\win-x64\sayso_webrtc_apm.lib
   ```
3. Continue with [Build the app and check it](#build-the-app-and-check-it).

**CI.** The build job in `.github/workflows/release-windows.yml` checks out
with `lfs: true`, so the release runner gets the library with the code.

**Not in GitHub Releases.** Don't publish the library as a GitHub Release
asset: this repo's releases feed the app's auto-updater.

## Rebuilding it yourself

Do this to move to a newer WebRTC, or to reproduce the committed library. It
is a one-off job: the first download and build take a few hours and about
30 GB of disk. After that, a rebuild takes minutes. When it is done, commit
the new library as in [Commit a rebuilt library](#commit-a-rebuilt-library).

### Prerequisites

- **Windows 10 or 11, x64.** Use a drive with at least 30 GB free.
- **Visual Studio 2022** (Community, Professional or Build Tools) with the
  **Desktop development with C++** workload. Check that **MSVC v143** and
  **Windows 11 SDK 10.0.26100** are selected.
  - Use VS 2022, not VS 2026. Your release workflow builds on the
    `windows-2022` runner, and MSVC can only link a library built with the
    same or an older Visual Studio. A library built with VS 2026 fails to
    link in CI.
  - Use a VS 2022 update that is the same as, or older than, the one on the
    runner image (17.14 at the time of writing).
- **Debugging Tools for Windows.** WebRTC's build needs them. Open
  *Settings → Apps → Windows Software Development Kit 10.0.26100 → Modify*,
  choose *Change*, and tick **Debugging Tools for Windows**.
- **Git for Windows**, with Git LFS set up (`git lfs install`).
- **Python 3.11** for the app's own native build (node-gyp 9, same as CI).
  WebRTC brings its own Python for its part of the build.

### 1. Install depot_tools

depot_tools is Google's tool set for fetching and building WebRTC.

```powershell
git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git C:\depot_tools
$env:Path = "C:\depot_tools;$env:Path"
$env:DEPOT_TOOLS_WIN_TOOLCHAIN = "0"   # use your installed Visual Studio
gclient                                 # first run installs its tools
```

Git settings recommended by Google for large checkouts on Windows:

```powershell
git config --global core.autocrlf false
git config --global core.filemode false
git config --global core.fscache true
git config --global core.preloadindex true
```

### 2. Get WebRTC at the tested revision

```powershell
mkdir D:\webrtc-checkout
cd D:\webrtc-checkout
fetch --nohooks webrtc
cd src
git checkout d943abd3750afb3fb6c956a4bafc4ef294b55198
gclient sync -D
```

`gclient sync` downloads WebRTC's dependencies and its clang compiler. It is
the slow step.

Stay on `d943abd3`: the wrapper is tested against it. A newer revision
usually works, but may need small changes in `sayso_apm.cc`. If you change
revision, test it as in [Build the app and check it](#build-the-app-and-check-it).

### 3. Build the library

From the repo root, in the same PowerShell window:

```powershell
.\electron\native-audio\scripts\build-webrtc-apm.ps1 `
    -WebRtcSrc D:\webrtc-checkout\src `
    -DepotTools C:\depot_tools
```

The script:

- finds your Visual Studio 2022 install;
- copies the wrapper into the checkout as `//sayso_apm`;
- builds only the wrapper and the WebRTC code it needs (x64 Release, static
  CRT, MSVC's C++ library);
- copies the result to `deps/webrtc_apm/win-x64/sayso_webrtc_apm.lib`;
- regenerates `THIRD_PARTY_NOTICES.txt`, and stops if the library contains a
  component without a known licence.

It ends with two lines like these:

```
WebRTC APM static library -> ...\electron\native-audio\deps\webrtc_apm\win-x64
Third-party notices (10 components) -> ...\electron\native-audio\THIRD_PARTY_NOTICES.txt
```

Your file will not match our SHA-256. WebRTC builds are not byte-for-byte
reproducible, and that is expected. Check the build by running the app instead.

If `THIRD_PARTY_NOTICES.txt` changed (for example, after a revision change),
commit it with the code.

The script also copies `sayso_apm.h` into `deps/`. The build does not use that
copy; it reads the header from `third_party/sayso_webrtc_apm/`.

## Commit a rebuilt library

The script writes the new library to the committed path, so commit it from the
repo root as usual (with Git LFS installed):

```powershell
git add electron/native-audio/deps/webrtc_apm/win-x64/sayso_webrtc_apm.lib electron/native-audio/THIRD_PARTY_NOTICES.txt
git lfs ls-files     # must list sayso_webrtc_apm.lib: stored in LFS, not as a plain git blob
git commit -m "build(native-audio): rebuild WebRTC library at <revision>"
git push
```

`.gitattributes` sends the file to Git LFS; `.gitignore` keeps the rest of
`deps/` (the header copy) out of git. CI needs no change. As above, keep it out
of GitHub Releases. When you change WebRTC revision, also update the revision in
this guide.

## Build the app and check it

1. Build the native module from the repo root, the same way CI does:
   ```powershell
   cd electron\native-audio; npm ci; cd ..\..
   npm run rebuild-native-win
   ```
   To make a missing library an error in this build too, set
   `$env:SAYSO_REQUIRE_WEBRTC_APM="1"` first. Packaging sets it automatically.
2. Start the app with echo cancellation on for this run (no code change is
   needed):
   ```powershell
   $env:SAYSO_AEC_ENABLE="1"
   npm run dev      # or launch your packaged build from this window
   ```
3. Start a call on speakers and check the app log:

   | Log line | Meaning |
   |---|---|
   | `[NativeAudio] lifecycle: aec_started` | Echo cancellation started with the mic. |
   | `[NativeAudio] lifecycle: aec_engine_ready engine=webrtc_aec3` | **The library is linked and working.** |
   | `[NativeAudio] lifecycle: aec_stats … mode=aec … reduction_db=…` | Once a minute while the prospect's audio plays. |
   | `aec_engine_unavailable reason=not_built` | The module was built **without** the library. Check the path and rebuild. |

4. Speaker test: mute yourself in the meeting app, stay silent and let the
   other person talk. Your mic should produce no transcript.

## Troubleshooting

| Problem | Cause and fix |
|---|---|
| The build says `the WebRTC echo-cancellation library is a Git LFS pointer` | The repo was cloned or pulled without Git LFS. Run `git lfs install`, then `git lfs pull`, and rebuild. |
| Linker error `LNK1107: invalid or corrupt file` on `sayso_webrtc_apm.lib` | Same cause: a Git LFS pointer instead of the library. Run `git lfs pull`. |
| Packaging stops with `the WebRTC echo-cancellation library is missing` | The `.lib` is not at `deps/webrtc_apm/win-x64/`: run `git lfs pull`. In CI, check `lfs: true` on the build job's checkout. For an emergency build without echo cancellation only: `SAYSO_ALLOW_BUILD_WITHOUT_AEC=1`. |
| Linker errors about a newer compiler, `LNK1143`, or `C1900`/IPDB mismatch | The library was built with a newer Visual Studio than the one linking it. Rebuild it with VS 2022, no newer than CI's. |
| Linker errors `LNK2038` mismatch for `RuntimeLibrary` or `_ITERATOR_DEBUG_LEVEL` | The library was not built by the script (wrong CRT or a debug build). Use `build-webrtc-apm.ps1`; it sets the right options. |
| The script stops with `Visual Studio 2022 with C++ tools ... is not installed` | Install the *Desktop development with C++* workload. |
| `gn gen` or `gclient sync` fails about the Windows SDK or debuggers | Install **Debugging Tools for Windows** (see Prerequisites). Check that `DEPOT_TOOLS_WIN_TOOLCHAIN=0` is set. |
| The script stops with `Unrecognised third-party object` | A newer WebRTC revision pulled in a new component. Add it, with its licence file, to the `$components` table in the script. |
| node-gyp fails with `No module named 'distutils'` | node-gyp 9 needs Python 3.11, as in CI. Set `$env:npm_config_python` to a Python 3.11 `python.exe`. |
| The app logs `not_built` although the file is there | The module was built before the file was added. Run `npm run rebuild-native-win` again. |
