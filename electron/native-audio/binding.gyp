{
  "targets": [
    {
      "target_name": "native_audio",
      "variables": {
        "webrtc_apm_win": "<!(node scripts/webrtc-apm-status.js)",
        "aec_diag_recording": "<!(node -p \"process.env.SAYSO_AEC_DIAG_RECORDING === '1' ? 1 : 0\")"
      },
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ],
      "conditions": [
        ["OS=='mac'", {
          "sources": [ "src/audio_device_manager.mm" ],
          "link_settings": {
            "libraries": [
              "-framework CoreAudio",
              "-framework CoreFoundation",
              "-framework Foundation",
              "-framework ScreenCaptureKit",
              "-framework AVFoundation",
              "-framework CoreMedia"
            ]
          },
          # Must stay below 13.0 (the lowest @available check in
          # audio_device_manager.mm). Clang treats an @available(macOS X, *)
          # check as compile-time-true, and dead-code-eliminates the else
          # branch, whenever the deployment target already guarantees X.
          # At 13.0 that silently erased the macOS-12 fallback path
          # (SAYSO-A3): capturesAudio ran unconditionally and crashed on
          # unrecognized selector. The app-level launch gate in main.ts is
          # the primary defense against pre-13 users reaching this code at
          # all; this keeps the fallback itself real as defense in depth.
          "xcode_settings": {
            "OTHER_CPLUSPLUSFLAGS": ["-std=c++20", "-stdlib=libc++"],
            "MACOSX_DEPLOYMENT_TARGET": "11.0"
          }
        }],
        ["OS=='win'", {
          "sources": [
            "src/audio_device_manager_win.cpp",
            "src/aec/aec_engine.cc",
            "src/aec/aec_pipeline.cc",
            "src/aec/audio_format.cc",
            "src/aec/resampler.cc"
          ],
          "include_dirs": [ "third_party/sayso_webrtc_apm" ],
          # Echo cancellation at launch, before JS calls
          # setEchoCancellationEnabled: 0 = off (the mic exactly as before),
          # 1 = on. See docs/ECHO_CANCELLATION_SWITCH.md.
          "defines": [ "SAYSO_AEC_DEFAULT_ON=0" ],
          "link_settings": {
            "libraries": [
              "ole32.lib",
              "mmdevapi.lib",
              "ksuser.lib"
            ]
          },
          "msvs_settings": {
            "VCCLCompilerTool": {
              "ExceptionHandling": 1,
              "AdditionalOptions": ["/std:c++20"]
            }
          },
          # WebRTC AEC3, linked statically when
          # scripts/build-webrtc-apm.ps1 has produced the library. Without it
          # the addon still builds and the echo canceller reports
          # "aec_engine_unavailable reason=not_built", passing the mic through.
          "conditions": [
            # Diagnostic call recording (reference / raw mic / clean mic WAVs)
            # is compiled into developer builds only: set
            # SAYSO_AEC_DIAG_RECORDING=1 when building. Release packaging
            # clears it (scripts/native-build-env.js).
            ["aec_diag_recording==1", {
              "defines": [ "SAYSO_AEC_DIAG_RECORDING" ],
              "sources": [ "src/aec/diagnostic_recorder.cc" ]
            }],
            ["webrtc_apm_win==1", {
              "defines": [ "SAYSO_HAVE_WEBRTC_APM" ],
              "link_settings": {
                "libraries": [
                  "<(module_root_dir)/deps/webrtc_apm/win-x64/sayso_webrtc_apm.lib",
                  "winmm.lib",
                  "user32.lib"
                ]
              }
            }]
          ]
        }]
      ]
    }
  ]
}
