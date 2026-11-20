# Builds aec_wav_harness.exe with the same MSVC and the same AEC sources the
# addon uses, linking the WebRTC static library when it has been built
# (scripts/build-webrtc-apm.ps1). Output: build/aec_wav_harness.exe here.

# cl and vcvars report on stderr; exit codes are checked instead.
$ErrorActionPreference = "Continue"
$here = $PSScriptRoot
$nativeAudio = Resolve-Path (Join-Path $here "../..")
$lib = Join-Path $nativeAudio "deps/webrtc_apm/win-x64/sayso_webrtc_apm.lib"
$out = Join-Path $here "build"
New-Item -ItemType Directory -Force $out -ErrorAction Stop | Out-Null

# Visual Studio 2022, like the WebRTC library and the client's release builds.
$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
$vsPath = & $vswhere -version "[17.0,18.0)" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if (-not $vsPath) { throw "Visual Studio 2022 with C++ tools is not installed" }
$vcvars = Join-Path $vsPath "VC\Auxiliary\Build\vcvars64.bat"
$env:Path = "$(Split-Path $vswhere);$env:Path"  # vcvars looks vswhere up on PATH

$sources = @("aec_engine.cc", "aec_pipeline.cc", "audio_format.cc", "diagnostic_recorder.cc", "resampler.cc") |
  ForEach-Object { "`"$(Join-Path $nativeAudio "src/aec/$_")`"" }
$defines = ""
$libs = ""
if (Test-Path $lib) {
  $defines = "/DSAYSO_HAVE_WEBRTC_APM"
  $libs = "`"$lib`" winmm.lib user32.lib"
} else {
  Write-Host "WebRTC library not built: harness will run the pass-through path only"
}

# /MT matches the static CRT the WebRTC library and the addon use.
$cmd = "`"$vcvars`" >nul && cl /nologo /std:c++20 /O2 /MT /EHsc /DSAYSO_AEC_DIAG_RECORDING $defines " +
       "/I`"$(Join-Path $nativeAudio 'third_party/sayso_webrtc_apm')`" " +
       "`"$(Join-Path $here 'aec_wav_harness.cc')`" $($sources -join ' ') $libs " +
       "/Fo`"$out\\`" /Fe`"$(Join-Path $out 'aec_wav_harness.exe')`""
cmd /c $cmd
if ($LASTEXITCODE -ne 0) { throw "harness build failed ($LASTEXITCODE)" }
Write-Host "Built $(Join-Path $out 'aec_wav_harness.exe')"
