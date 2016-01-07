# node-opencv 

[![Build Status](https://secure.travis-ci.org/peterbraden/node-opencv.png)](http://travis-ci.org/peterbraden/node-opencv)


[OpenCV](http://opencv.org) bindings for Node.js. OpenCV is
the defacto computer vision library - by interfacing with it natively in node,
we get powerful real time vision in js.

People are using node-opencv to fly control quadrocoptors, detect faces from
webcam images and annotate video streams. If you're using it for something
cool, I'd love to hear about it!

## Install

You'll need OpenCV 2.3.1 or newer installed before installing node-opencv.

## Specific for Windows
1. Download Install opencv @ - (I used version 2.4.4)
http://opencv.org/downloads.html
Put it in c:\opencv

2. Install python version 2.7 @
http://www.python.org/download/releases/2.7/
put it in c:\python27

3. install pkg-config by downloading the all in one bundle @ - (I used Gtk+ 3.6.4)
http://www.gtk.org/download/win64.php
put it in c:\pkg-config

4. Add the following to your path variables
C:\pkg-config\bin;C:\OpenCV\build\x64\vc11\bin;

5. Install visual-studio in 4 steps

  - install Visual C++ 2010 Express
  
  - install Windows SDK for windows 7 and .net framework 4
  
  - install Visual Studio 2010 Service Pack 1
  
  - install Visual C++ 2010 Service Pack 1 Compiler


6. Download npeterbraden/node-opencv fork
git clone https://github.com/peterbraden/node-opencv

7. edit file src/Matrix.cpp
put "inline double round( double d ) { return floor( d + 0.5);}" below "cv::Rect* setRect(Local<Object> objRect, cv::Rect &result);"

8. run npm install

Then:

```bash
$ npm install opencv
```

## Examples
Run the examples from the parent directory.

### Face Detection

```javascript
cv.readImage("./examples/files/mona.png", function(err, im){
  im.detectObject(cv.FACE_CASCADE, {}, function(err, faces){
    for (var i=0;i<faces.length; i++){
      var x = faces[i]
      im.ellipse(x.x + x.width/2, x.y + x.height/2, x.width/2, x.height/2);
    }
    im.save('./out.jpg');
  });
})
```


## API Documentation

### Matrix

The [matrix](http://opencv.jp/opencv-2svn_org/cpp/core_basic_structures.html#mat) is the most useful
base datastructure in OpenCV. Things like images are just matrices of pixels.

#### Creation

```javascript
new Matrix(rows, cols)
```

Or if you're thinking of a Matrix as an image:

```javascript
new Matrix(height, width)
```

Or you can use opencv to read in image files. Supported formats are in the OpenCV docs, but jpgs etc are supported.

```javascript
cv.readImage(filename, function(err, mat){
  ...
})

cv.readImage(buffer, function(err, mat){
  ...
})
```

If you need to pipe data into an image, you can use an ImageDataStream:

```javascript
var s = new cv.ImageDataStream()

s.on('load', function(matrix){
  ...
})

fs.createReadStream('./examples/files/mona.png').pipe(s);
```

If however, you have a series of images, and you wish to stream them into a
stream of Matrices, you can use an ImageStream. Thus:

```javascript
var s = new cv.ImageStream()

s.on('data', function(matrix){
   ...
})

ardrone.createPngStream().pipe(s);
```

Note: Each 'data' event into the ImageStream should be a complete image buffer.



#### Accessing Data

```javascript
var mat = new cv.Matrix.Eye(4,4); // Create identity matrix

mat.get(0,0) // 1

mat.row(0)  // [1,0,0,0]
mat.col(4)  // [0,0,0,1]
```

##### Save

```javascript
mat.save('./pic.jpg')
```

or:

```javascript
var buff = mat.toBuffer()
```

#### Image Processing

```javascript
im.convertGrayscale()
im.canny(5, 300)
im.houghLinesP()
```


#### Simple Drawing

```javascript
im.ellipse(x, y)
im.line([x1,y1], [x2, y2])
```

#### Object Detection

There is a shortcut method for 
[Viola-Jones Haar Cascade](http://www.cognotics.com/opencv/servo_2007_series/part_2/sidebar.html) object 
detection. This can be used for face detection etc.

```javascript
mat.detectObject(haar_cascade_xml, opts, function(err, matches){})
```

For convenience in face detection, cv.FACE_CASCADE is a cascade that can be used for frontal face detection.

Also:

```javascript
mat.goodFeaturesToTrack
```

#### Contours

```javascript
mat.findCountours
mat.drawContour
mat.drawAllContours
```

### Using Contours

`findContours` returns a `Contours` collection object, not a native array. This object provides
functions for accessing, computing with, and altering the contours contained in it.
See [relevant source code](src/Contours.cc) and [examples](examples/)

```javascript
var contours = im.findContours();

// Count of contours in the Contours object
contours.size();

// Count of corners(verticies) of contour `index`
contours.cornerCount(index);

// Access vertex data of contours
for(var c = 0; c < contours.size(); ++c) {
  console.log("Contour " + c);
  for(var i = 0; i < contours.cornerCount(c); ++i) {
    var point = contours.point(c, i);
    console.log("(" + point.x + "," + point.y + ")");
  }
}

// Computations of contour `index`
contours.area(index);
contours.arcLength(index, isClosed);
contours.boundingRect(index);
contours.minAreaRect(index);
contours.isConvex(index);
contours.fitEllipse(index);

// Destructively alter contour `index`
contours.approxPolyDP(index, epsilon, isClosed);
contours.convexHull(index, clockwise);
```

## MIT License
The library is distributed under the MIT License - if for some reason that
doesn't work for you please get in touch.

