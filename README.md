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


        var im = cv.readImage("./examples/test.jpg")

        im.faceDetect("./examples/haarcascade_frontalface_alt.xml", {}, function(err, faces){  
          for (var i=0;i<faces.length; i++){
            var x = faces[i]
            im.ellipse(x.x + x.width/2, x.y + x.height/2, x.width/2, x.height/2);
          }
          im.save('./out.jpg');   
                 
        });



## API Documentation

### Matrix

The [matrix](http://opencv.jp/opencv-2svn_org/cpp/core_basic_structures.html#mat) is the most useful
base datastructure in OpenCV. Things like images are just matrices of pixels.

#### Creation

        new Matrix(width, height)

Or you can use opencv to read in image files. Supported formats are in the OpenCV docs, but jpgs etc are supported.

        cv.readImage(filename, function(mat){
          ...
        })

        cv.readImage(buffer, function(mat){
          ...
        })

If you need to pipe data into an image, you can use an imagestream:

        var s = new cv.ImageStream()

        s.on('load', function(matrix){ 
          ...
        }) 

        fs.createReadStream('./examples/test.jpg').pipe(s);        

#### Accessors

#### Image Processing









## WIP

This is a WIP. I've never written C++ before so the code may be _interesting_ - if 
I'm doing stuff wrong please feel free to correct me.