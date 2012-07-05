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
      , "cflags" : ["-O3", "-Wall", "<!@(pkg-config --cflags opencv]"]  
      , "libraries" : ["<!@(pkg-config --libs opencv)"]

      }]
}

