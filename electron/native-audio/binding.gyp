{
  "targets": [
    {
      "target_name": "native_audio",
      "sources": [ "src/audio_device_manager.mm" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ],
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
        "OTHER_CFLAGS": ["-std=c++20", "-stdlib=libc++"],
        "MACOSX_DEPLOYMENT_TARGET": "13.0"
      }
    }
  ]
}
