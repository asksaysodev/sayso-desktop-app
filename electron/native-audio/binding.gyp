{
  "targets": [
    {
      "target_name": "native_audio",
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
          "xcode_settings": {
            "OTHER_CPLUSPLUSFLAGS": ["-std=c++20", "-stdlib=libc++"],
            "MACOSX_DEPLOYMENT_TARGET": "13.0"
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
