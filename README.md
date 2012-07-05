# node-opencv 

[![Build Status](https://secure.travis-ci.org/peterbraden/node-opencv.png)](http://travis-ci.org/peterbraden/node-opencv)


[OpenCV](http://opencv.willowgarage.com/wiki/) bindings for Node.js

## Install

You'll need OpenCV 2.3.1 installed.

Then:


        npm install opencv


Or to build the repo:


        node-gyp rebuild


## Examples

### Face Detection


        cv.readImage("./examples/test.jpg", function(err, im){
          im.detectObject("./data/haarcascade_frontalface_alt.xml", {}, function(err, faces){  
            for (var i=0;i<faces.length; i++){
              var x = faces[i]
              im.ellipse(x.x + x.width/2, x.y + x.height/2, x.width/2, x.height/2);
            }
            im.save('./out.jpg');   
                   
          });
        })



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
        
        var mat = new cv.Matrix.Eye(4,4); // Create identity matrix

        mat.get(0,0) // 1

        mat.row(0)  // [1,0,0,0]
        mat.col(4)  // [0,0,0,1]



#### Image Processing


#### Object Detection

There is a shortcut method for 
[Viola-Jones Haar Cascade](http://www.cognotics.com/opencv/servo_2007_series/part_2/sidebar.html) object 
detection. This can be used for face detection etc.


        mat.detectObject(haar_cascade_xml, opts, function(err, matches){})




## WIP

This is a WIP. I've never written C++ before so the code may be _interesting_ - if 
I'm doing stuff wrong please feel free to correct me.
