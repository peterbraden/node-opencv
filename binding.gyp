{
  "targets": [{ 
      "target_name": "opencv"
      , "sources": [ 
          "src/init.cc"
        , "src/Matrix.cc"
        , "src/OpenCV.cc"
        , "src/CascadeClassifierWrap.cc"
        , "src/Point.cc"
        , "src/VideoCaptureWrap.cc"
        ]
      , "conditions": [
        ['OS=="mac"', {
          # cflags on OS X are stupid and have to be defined like this
          'xcode_settings': {
            'OTHER_CFLAGS': [
              '<!@(pkg-config --cflags opencv)'
            ],
          }
        }, {
          'cflags': [
            '<!@(pkg-config --cflags opencv)'
            , 'Wall'
          ]
        }]        
      ]
  }]
}

