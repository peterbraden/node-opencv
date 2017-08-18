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
        "src/VideoWriterWrap.cc",
        "src/CamShift.cc",
        "src/HighGUI.cc",
        "src/FaceRecognizer.cc",
        "src/Features2d.cc",
        "src/BackgroundSubtractor.cc",
        "src/Constants.cc",
        "src/Calib3D.cc",
        "src/ImgProc.cc",
        "src/Stereo.cc",
        "src/LDAWrap.cc"
      ],

      "libraries": [
        "<!@(node utils/find-opencv.js --libs)"
      ],
      # For windows

      "include_dirs": [
        "<!@(node utils/find-opencv.js --cflags)",
        "<!(node -e \"require('nan')\")"
      ],

      "cflags!" : [ "-fno-exceptions"],
      "cflags_cc!": [ "-fno-rtti",  "-fno-exceptions"],

      "conditions": [
        [ "OS==\"linux\" or OS==\"freebsd\" or OS==\"openbsd\" or OS==\"solaris\" or OS==\"aix\"", {
            "cflags": [
              "<!@(node utils/find-opencv.js --cflags)",
              "-Wall"
            ]
        }],
        [ "OS==\"win\"", {
            "cflags": [
              "-Wall"
            ],
            "defines": [
                "WIN"
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
                "<!@(node utils/find-opencv.js --cflags)",
              ],
              "GCC_ENABLE_CPP_RTTI": "YES",
              "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
            }
          }
        ]
    ],

    "configurations": {
        # This is used for generating code coverage with the `--debug` argument
        "Debug": {
          "conditions": [
            ['OS=="linux"', {
              "cflags": ["-coverage"],
              "ldflags": ["-coverage"]
            }],
            ['OS=="mac"', {
              "xcode_settings": {
                "OTHER_CFLAGS": [
                  "-fprofile-arcs -ftest-coverage",
                ],
                "OTHER_LDFLAGS": [
                  "--coverage"
                ]
              }
            }]

          ]
        },
    }
  },
  {
      "target_name": "test_nativemat",

      "sources": [
        "test/nativemat.cc",
      ],

      "libraries": [
        "<!@(node utils/find-opencv.js --libs)",
      ],
      # For windows

      "include_dirs": [
        "<!@(node utils/find-opencv.js --cflags)",
        "<!(node -e \"require('nan')\")",
        "<!(node -e \"require('./include_dirs')\")"
      ],

      "cflags!" : [ "-fno-exceptions"],
      "cflags_cc!": [ "-fno-rtti",  "-fno-exceptions"],

      "conditions": [
        [ "OS==\"linux\"", {
            "cflags": [
              "<!@(node utils/find-opencv.js --cflags)",
              "-Wall"
            ]
        }],
        [ "OS==\"win\"", {
            "cflags": [
              "-Wall"
            ],
            "defines": [
                "WIN"
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
              "<!@(node utils/find-opencv.js --cflags)",
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
