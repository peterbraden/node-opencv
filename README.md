# node-opencv 

[![Build Status](https://secure.travis-ci.org/peterbraden/node-opencv.png)](http://travis-ci.org/peterbraden/node-opencv)


[OpenCV](http://opencv.willowgarage.com/wiki/) bindings for Node.js

## Install

You'll need OpenCV installed. I'm using v2.2 because I couldn't get 2.3 to compile, but it should theoretically work with 2.3

Then:


        npm install opencv


Or to build the repo:


        node-waf configure && node-waf build


## Examples

### Face Detection


        var im = new cv.Image("./examples/mona.jpg")
        , face_cascade = new cv.CascadeClassifier("./examples/haarcascade_frontalface_alt.xml")

        var faces = face_cascade.detectMultiScale(im, function(err, faces){
          
          for (var i=0;i<faces.length; i++){
            var x = faces[i]
            im.ellipse(x.x + x.width/2, x.y + x.height/2, x.width/2, x.height/2);
          }
          im.save('./out.jpg');   
                 
        }, 1.1, 2, [30, 30]);





## WIP

This is a WIP. I've never written C++ before so the code may be _interesting_ - if 
I'm doing stuff wrong please feel free to correct me.

## Links (WIP these are resources that I'm working from)

- http://syskall.com/how-to-write-your-own-native-nodejs-extension
- https://github.com/pquerna/node-extension-examples/blob/master/helloworld_eio/helloworld_eio.cc
- http://code.google.com/p/waf/
- http://www.cs.iit.edu/~agam/cs512/lect-notes/opencv-intro/opencv-intro.html
- http://opencv.willowgarage.com/documentation/cpp/core_basic_structures.html#point
- http://bespin.cz/~ondras/html/classv8_1_1NumberObject.html
- http://bespin.cz/~ondras/html/index.html
- http://v8cgi.googlecode.com/svn-history/r963/trunk/src/macros.h
