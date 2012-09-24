{
  "targets": [{ 
      "target_name": "opencv"
      , "sources": [ 
          "src/init.cc"
        , "src/Matrix.cc"
        , "src/OpenCV.cc"
        , "src/CascadeClassifierWrap.cc"
        , "src/Contours.cc"
        , "src/Point.cc"
        , "src/VideoCaptureWrap.cc"
        ]
      , 'libraries': [
          '<!@(pkg-config --libs opencv)'
        ]
      , "conditions": [
        ['OS=="mac"', {
          # cflags on OS X are stupid and have to be defined like this
          'xcode_settings': {
            'OTHER_CFLAGS': [
              '<!@(pkg-config --cflags opencv)'
            ]
            , "GCC_ENABLE_CPP_RTTI": "YES"
            , "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
          },
        }, {
          'cflags': [
            '<!@(pkg-config --cflags opencv)'
            , '-Wall'
          ]
          , 'cflags!' : [ '-fno-exceptions']
          , 'cflags_cc!': [ '-fno-rtti',  '-fno-exceptions']
        }]        
      
      , ['OS=="linux"', {
        'cflags': ['-ldl', '-Wall', '<!@(pkg-config --cflags opencv)']
        , 'cflags!' : ['-fno-exceptions']
        , 'cflags_cc!' : ['-fno-rtti',  '-fno-exceptions']

      }]
    ]
  }]
}

