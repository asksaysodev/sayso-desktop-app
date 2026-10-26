{
  "targets": [
    {
      "target_name": "mic_activity",
      "sources": [ "src/mic_activity.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ],
      "conditions": [
        ["OS=='mac'", {
          "sources": [ "src/mic_activity_mac.mm" ],
          "link_settings": {
            "libraries": [
              "-framework AppKit",
              "-framework CoreAudio",
              "-framework Foundation"
            ]
          },
          # 13.0 = the app's LSMinimumSystemVersion. Must stay below 14.2 so the
          # @available(macOS 14.2, *) checks in mic_activity_mac.mm stay runtime
          # checks: the module loads on 13-14.1 and reports "unsupported".
          "xcode_settings": {
            "OTHER_CPLUSPLUSFLAGS": ["-std=c++20", "-stdlib=libc++"],
            "CLANG_ENABLE_OBJC_ARC": "YES",
            "MACOSX_DEPLOYMENT_TARGET": "13.0"
          }
        }],
        ["OS=='win'", {
          "sources": [ "src/mic_activity_win.cpp" ],
          "link_settings": {
            "libraries": [ "ole32.lib" ]
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
