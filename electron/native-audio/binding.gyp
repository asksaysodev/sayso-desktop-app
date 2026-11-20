{
  "targets": [
    {
      "target_name": "native_audio",
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ],
      "conditions": [
        ["OS=='mac'", {
          "sources": [ "src/audio_device_manager.mm", "src/echo/echo_engine.cpp", "src/echo/echo_pipeline.cpp" ],
          "variables": {
            "apm_lib%": "<!(test -f third_party/webrtc-apm/lib/<(target_arch)/libsayso_apm.a && echo yes || echo no)"
          },
          "conditions": [
            ["apm_lib=='yes'", {
              "defines": [ "SAYSO_HAVE_APM=1" ],
              "include_dirs": [ "third_party/webrtc-apm/include" ],
              "link_settings": {
                "libraries": [ "<(module_root_dir)/third_party/webrtc-apm/lib/<(target_arch)/libsayso_apm.a" ]
              }
            }]
          ],
          "link_settings": {
            "libraries": [
              "-framework CoreAudio",
              "-framework CoreFoundation",
              "-framework Foundation",
              "-framework ScreenCaptureKit",
              "-framework AVFoundation",
              "-framework AppKit",
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
          "sources": [ "src/audio_device_manager_win.cpp" ],
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
          }
        }]
      ]
    }
  ]
}
