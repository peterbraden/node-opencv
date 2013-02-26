# node-opencv 

[![Build Status](https://secure.travis-ci.org/peterbraden/node-opencv.png)](http://travis-ci.org/peterbraden/node-opencv)


[OpenCV](http://opencv.willowgarage.com/wiki/) bindings for Node.js. OpenCV is the defacto computer vision library - by interfacing with it natively in node, we get powerful real time vision in js.

People are using node-opencv to fly control quadrocoptors, detect faces from webcam images and annotate video streams. If you're using it for something cool, I'd love to hear about it!

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

        new Matrix(rows, cols)

Or if you're thinking of a Matrix as an image:

        new Matrix(height, width)

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

#### Accessing Data

        var mat = new cv.Matrix.Eye(4,4); // Create identity matrix

        mat.get(0,0) // 1

        mat.row(0)  // [1,0,0,0]
        mat.col(4)  // [0,0,0,1]


##### Save

        mat.save('./pic.jpg')

or:

        var buff = mat.toBuffer()


#### Image Processing

        im.convertGrayscale()
        im.canny(5, 300)
        im.houghLinesP()



#### Simple Drawing

        im.ellipse(x, y)
        im.line([x1,y1], [x2, y2])


#### Object Detection

There is a shortcut method for 
[Viola-Jones Haar Cascade](http://www.cognotics.com/opencv/servo_2007_series/part_2/sidebar.html) object 
detection. This can be used for face detection etc.


        mat.detectObject(haar_cascade_xml, opts, function(err, matches){})


Also:

        mat.goodFeaturesToTrack


#### Contours

        mat.findCountours
        mat.drawContour
        mat.drawAllContours

### Using Contours

`findContours` returns a `Contours` collection object, not a native array. This object provides
functions for accessing, computing with, and altering the contours contained in it.
See [relevant source code](src/Contours.cc) and [examples](examples/)

        var contours = im.findContours;

        # Count of contours in the Contours object
        contours.size();

        # Count of corners(verticies) of contour `index`
        contours.cornerCount(index);

        # Access vertex data of contours
        for(var c = 0; c < contours.size(); ++c) {
          console.log("Contour " + c);
          for(var i = 0; i < contours.cornerCount(c); ++i) {
            var point = contours.point(c, i);
            console.log("(" + point.x + "," + point.y + ")");"
          }
        }

        # Computations of contour `index`
        contours.area(index);
        contours.arcLength(index, isClosed);
        contours.boundingRect(index);
        contours.minAreaRect(index);
        contours.isConvex(index);

        # Destructively alter contour `index`
        contours.approxPolyDP(index, epsilon, isClosed);
        contours.convexHull(index, clockwise);

## MIT License
The library is distributed under the MIT License - if for some reason that 
doesn't work for you please get in touch.

## Changelog


#### 0.0.10

- Bug Fixes
- @Contra added code that allows thickness and color args for ellipse
- Camshift Support
- @jtlebi added bindings for erode, gaussianBlur, arcLength, approxPolyDP, isConvex, cornerCount
- @gluxon added bindings for inRange

Thanks everyone!

#### 0.0.9

- toBuffer can now take a callback and be run async (re #21)
