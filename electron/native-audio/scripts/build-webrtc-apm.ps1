# Builds WebRTC AudioProcessing (AEC3) as ONE static library and drops it in
# electron/native-audio/deps/webrtc_apm/win-x64/, where binding.gyp links it
# into native_audio.node. No WebRTC DLL ships. The library is committed with
# Git LFS at that path: commit the rebuilt one as described in
# docs/WEBRTC_LIBRARY_BUILD.md ("Commit a rebuilt library"), never as a GitHub
# Release asset (releases feed the app's auto-updater).
#
# WebRTC only builds with its own clang-cl, so this runs inside a WebRTC
# checkout. Two settings keep the result linkable by node-gyp's MSVC build:
#   use_custom_libcxx=false  -> MSVC's C++ library, not WebRTC's bundled libc++
#   is_debug=false, non-component -> static CRT (/MT), matching node-gyp Release
# The addon only calls the C interface in sayso_apm.h, so the two compilers
# never share C++ types.
#
# Visual Studio 2022 by default: the client's release workflow builds on a
# windows-2022 runner, and a library can only be linked by an MSVC at least as
# new as the one it was built with. Build with the same (or an older) VS 2022
# release than that runner image carries.
#
# Usage (PowerShell):
#   .\electron\native-audio\scripts\build-webrtc-apm.ps1 `
#       -WebRtcSrc E:\webrtc-checkout\src -DepotTools C:\webrtc-build\depot_tools

param(
  [Parameter(Mandatory = $true)][string]$WebRtcSrc,
  [Parameter(Mandatory = $true)][string]$DepotTools,
  [ValidateSet("2022", "2026")][string]$VsVersion = "2022",
  # One output folder per Visual Studio version, so their objects never mix.
  [string]$OutDir = "out/sayso_apm_x64_vs$VsVersion"
)

# Native tools (gn, ninja) report progress on stderr, which Windows PowerShell
# turns into terminating errors under "Stop"; their exit codes are checked
# explicitly instead, and cmdlets below pass -ErrorAction Stop.
$ErrorActionPreference = "Continue"
$nativeAudio = Split-Path -Parent $PSScriptRoot
$wrapper = Join-Path $nativeAudio "third_party/sayso_webrtc_apm"
$dest = Join-Path $nativeAudio "deps/webrtc_apm/win-x64"

$env:Path = "$DepotTools;$env:Path"
# Use a locally installed Visual Studio of the requested version. WebRTC only
# searches %ProgramFiles% and takes the newest one it finds, so point it at
# exactly the install vswhere reports for $VsVersion (Build Tools commonly
# live under Program Files (x86)) and hide any other version.
$env:DEPOT_TOOLS_WIN_TOOLCHAIN = "0"
$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
$range = @{ '2022' = '[17.0,18.0)'; '2026' = '[18.0,19.0)' }[$VsVersion]
$vsPath = & $vswhere -version $range -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if (-not $vsPath) { throw "Visual Studio $VsVersion with C++ tools (Desktop development with C++) is not installed" }
foreach ($year in @('2017', '2019', '2022', '2026')) { Remove-Item "env:vs$($year)_install" -ErrorAction SilentlyContinue }
Set-Item -Path "env:vs$($VsVersion)_install" -Value $vsPath
Write-Host "Visual Studio $VsVersion -> $vsPath"

$target = Join-Path $WebRtcSrc "sayso_apm"
New-Item -ItemType Directory -Force $target -ErrorAction Stop | Out-Null
Copy-Item (Join-Path $wrapper "*") $target -Force -ErrorAction Stop

# Written to args.gn rather than passed as --args: Windows PowerShell strips
# the quotes around "x64" when handing arguments to a native program.
$gnArgs = @(
  'is_debug=false',
  'is_component_build=false',
  'target_cpu="x64"',
  'use_custom_libcxx=false',
  'rtc_include_tests=false',
  'rtc_build_examples=false',
  'rtc_build_tools=false',
  'rtc_enable_protobuf=false',
  'treat_warnings_as_errors=false',
  'symbol_level=0'
)
$outPath = Join-Path $WebRtcSrc $OutDir
New-Item -ItemType Directory -Force $outPath -ErrorAction Stop | Out-Null
Set-Content -Path (Join-Path $outPath "args.gn") -Value $gnArgs -Encoding ascii -ErrorAction Stop

Push-Location $WebRtcSrc
try {
  # Root the build graph at our wrapper: only it and its WebRTC dependencies
  # are generated, and WebRTC's own BUILD.gn files stay untouched.
  & gn gen $OutDir --root-target=//sayso_apm
  if ($LASTEXITCODE -ne 0) { throw "gn gen failed ($LASTEXITCODE)" }
  & autoninja -C $OutDir obj/sayso_apm/sayso_apm.lib
  if ($LASTEXITCODE -ne 0) { throw "ninja failed ($LASTEXITCODE)" }
} finally {
  Pop-Location
}

New-Item -ItemType Directory -Force $dest -ErrorAction Stop | Out-Null
$builtLib = Join-Path $WebRtcSrc "$OutDir/obj/sayso_apm/sayso_apm.lib"
Copy-Item $builtLib (Join-Path $dest "sayso_webrtc_apm.lib") -Force -ErrorAction Stop
Copy-Item (Join-Path $wrapper "sayso_apm.h") $dest -Force -ErrorAction Stop
Write-Host "WebRTC APM static library -> $dest"

# ── Third-party notices ──────────────────────────────────────────────────────
# Everything linked into the library is listed, and every object is matched to
# the component (and licence) it came from. An object from a component not in
# this table stops the build, so the notices can never fall behind the code
# that actually ships. The result lands next to native_audio.node and ships
# with the app.
$components = @(
  # Order = order in the notices file. Prefix = path of its objects in the .lib.
  @{ Name = "WebRTC"; Prefix = ""; Licenses = @("LICENSE", "PATENTS") },
  @{ Name = "Abseil"; Prefix = "obj/third_party/abseil-cpp/"; Licenses = @("third_party/abseil-cpp/LICENSE") },
  @{ Name = "Perfetto"; Prefix = "obj/third_party/perfetto/"; Licenses = @("third_party/perfetto/LICENSE") },
  @{ Name = "RNNoise"; Prefix = "obj/third_party/rnnoise/"; Licenses = @("third_party/rnnoise/COPYING") },
  @{ Name = "PFFFT"; Prefix = "obj/third_party/pffft/"; Licenses = @("third_party/pffft/LICENSE") },
  @{ Name = "Ooura FFT"; Prefix = "obj/common_audio/third_party/ooura/"; Licenses = @("common_audio/third_party/ooura/LICENSE") },
  @{ Name = "spl_sqrt_floor"; Prefix = "obj/common_audio/third_party/spl_sqrt_floor/"; Licenses = @("common_audio/third_party/spl_sqrt_floor/LICENSE") },
  @{ Name = "WebRTC fft"; Prefix = "obj/modules/third_party/fft/"; Licenses = @("modules/third_party/fft/LICENSE") },
  @{ Name = "cxx (Rust/C++ bridge)"; Prefix = "obj/build/rust/cxx_cppdeps/"; Licenses = @("third_party/rust/chromium_crates_io/vendor/cxx-v1/LICENSE-APACHE") },
  @{ Name = "Chromium build support"; Prefix = "obj/build/"; Licenses = @("buildtools/LICENSE") }
)

$lldLink = Join-Path $WebRtcSrc "third_party/llvm-build/Release+Asserts/bin/lld-link.exe"
$members = & $lldLink /lib /list $builtLib
if ($LASTEXITCODE -ne 0 -or -not $members) { throw "could not list $builtLib" }

$used = @{}
foreach ($member in $members) {
  $m = $member -replace '\\', '/'
  # Longest matching prefix wins, so obj/build/rust/cxx_cppdeps/ beats obj/build/.
  $match = $components | Where-Object { $_.Prefix -ne "" -and $m.StartsWith($_.Prefix) } |
    Sort-Object { $_.Prefix.Length } -Descending | Select-Object -First 1
  if (-not $match) {
    if ($m -match 'third_party/' -or $m.StartsWith("obj/build/")) {
      throw "Unrecognised third-party object in the library: $m. Add its component and licence to build-webrtc-apm.ps1."
    }
    $match = $components[0]  # WebRTC's own code (and our wrapper)
  }
  $used[$match.Name] = $true
}

$revision = (& git -C $WebRtcSrc rev-parse HEAD).Trim()
$lines = @(
  "Third-party software in native_audio.node (echo cancellation)",
  "",
  "native_audio.node statically links WebRTC audio processing, built from",
  "https://webrtc.googlesource.com/src at revision $revision,",
  "together with the components below. Each is reproduced under its own licence.",
  ""
)
foreach ($c in $components) {
  if (-not $used[$c.Name]) { continue }
  $lines += ("=" * 78)
  $lines += $c.Name
  $lines += ("=" * 78)
  foreach ($license in $c.Licenses) {
    $lines += ""
    $lines += (Get-Content (Join-Path $WebRtcSrc $license) -ErrorAction Stop)
  }
  $lines += ""
}
$notices = Join-Path $nativeAudio "THIRD_PARTY_NOTICES.txt"
# UTF-8 without BOM, LF line endings, like the rest of the repository.
[System.IO.File]::WriteAllText($notices, (($lines -join "`n") + "`n"), (New-Object System.Text.UTF8Encoding($false)))
Write-Host "Third-party notices ($(@($used.Keys).Count) components) -> $notices"
Write-Host "Next: commit the library (Git LFS) and the notices; see docs/WEBRTC_LIBRARY_BUILD.md, 'Commit a rebuilt library'."
