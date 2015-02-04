{
  "targets": [{
    "target_name": "opencv",
      "sources": [
        "src/init.cc",
        "src/Matrix.cc",
        "src/OpenCV.cc",
        "src/CascadeClassifierWrap.cc",
        "src/Contours.cc",
        "src/Point.cc",
        "src/VideoCaptureWrap.cc",
        "src/CamShift.cc",
        "src/HighGUI.cc",
        "src/FaceRecognizer.cc",
        "src/BackgroundSubtractor.cc",
        "src/Constants.cc"
      ],

      "libraries": [
        "<!@(pkg-config --libs opencv)"
      ],

      # For windows

      "include_dirs": [
        "<!@(pkg-config --cflags opencv)",
        "<!(node -e \"require('nan')\")"
      ],

      "cflags!" : [ "-fno-exceptions"],
      "cflags_cc!": [ "-fno-rtti",  "-fno-exceptions"],

      "conditions": [
        [ "OS==\"linux\"", {
            "cflags": [
              "<!@(pkg-config --cflags \"opencv >= 2.3.1\" )",
              "-Wall"
            ]
        }],
        [ "OS==\"win\"", {
            "cflags": [
              "<!@(pkg-config --cflags \"opencv >= 2.4.9\" )",
              "-Wall"
            ],
            "msvs_settings": {
              "VCCLCompilerTool": {
                "ExceptionHandling": "2",
                "DisableSpecificWarnings": [ "4530", "4506", "4244" ],
              },
            }
        }],
        [ # cflags on OS X are stupid and have to be defined like this
          "OS==\"mac\"", {
            "xcode_settings": {
            "OTHER_CFLAGS": [
              "-mmacosx-version-min=10.7",
            "-std=c++11",
            "-stdlib=libc++",
            "<!@(pkg-config --cflags opencv)"
              ],
            "GCC_ENABLE_CPP_RTTI": "YES",
            "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
          }
        }]
    ]
  },
    {
      "target_name": "action_after_build",
      "type": "none",
      "dependencies": [ "<(module_name)" ],
      "copies": [
      {
        "files": [ "<(PRODUCT_DIR)/<(module_name).node" ],
        "destination": "<(module_path)"
      }
      ]
    }]
}
