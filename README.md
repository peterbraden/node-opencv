# node-opencv

[![Build Status](https://secure.travis-ci.org/peterbraden/node-opencv.svg)](http://travis-ci.org/peterbraden/node-opencv)
[![Coverage](http://codecov.io/github/peterbraden/node-opencv/coverage.svg?branch=master)](https://codecov.io/gh/peterbraden/node-opencv)

[OpenCV](http://opencv.org) bindings for Node.js. OpenCV is
the defacto computer vision library - by interfacing with it natively in node,
we get powerful real time vision in js.

People are using node-opencv to fly control quadrocoptors, detect faces from
webcam images and annotate video streams. If you're using it for something
cool, I'd love to hear about it!

## Install

You'll need OpenCV 2.3.1 or newer installed before installing node-opencv.  Note
that OpenCV 3.x is not yet fully supported.

## Specific for macOS
Install OpenCV using brew
```bash
brew tap homebrew/science
brew install opencv
```


## Specific for Windows
1. Download and install OpenCV (Be sure to use a 2.4 version) @
http://opencv.org/releases.html
For these instructions we will assume OpenCV is put at C:\OpenCV, but you can
adjust accordingly.

2. If you haven't already, create a system variable called OPENCV_DIR and set it
   to C:\OpenCV\build\x64\vc12

   Make sure the "x64" part matches the version of NodeJS you are using.

   Also add the following to your system PATH
        ;%OPENCV_DIR%\bin

3. Install Visual Studio 2013. Make sure to get the C++ components.
   You can use a different edition, just make sure OpenCV supports it, and you
   set the "vcxx" part of the variables above to match.

4. Download peterbraden/node-opencv fork
git clone https://github.com/peterbraden/node-opencv

5. run npm install

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
base data structure in OpenCV. Things like images are just matrices of pixels.

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
[Viola-Jones Haar Cascade](http://docs.opencv.org/trunk/d7/d8b/tutorial_py_face_detection.html) object
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

#### Face Recognization

It requires to `train` then `predict`. For acceptable result, the face should be cropped, grayscaled and aligned, I ignore this part so that we may focus on the api usage.

** Please ensure your OpenCV 3.2+ is configured with contrib. MacPorts user may `port install opencv +contrib` **

```javascript
const fs = require('fs');
const path = require('path');
const cv = require('opencv');

function forEachFileInDir(dir, cb) {
  let f = fs.readdirSync(dir);
  f.forEach(function (fpath, index, array) {
    if (fpath != '.DS_Store')
     cb(path.join(dir, fpath));
  });
}

let dataDir = "./_training";
function trainIt (fr) {
  // if model existe, load it
  if ( fs.existsSync('./trained.xml') ) {
    fr.loadSync('./trained.xml');
    return;
  }

  // else train a model
  let samples = [];
  forEachFileInDir(dataDir, (f)=>{
      cv.readImage(f, function (err, im) {
          // Assume all training photo are named as id_xxx.jpg
          let labelNumber = parseInt(path.basename(f).substring(3));
          samples.push([labelNumber, im]);
      })
  })

  if ( samples.length > 3 ) {
    // There are async and sync version of training method:
    // .train(info, cb)
    //     cb : standard Nan::Callback
    //     info : [[intLabel,matrixImage],...])
    // .trainSync(info)
    fr.trainSync(samples);
    fr.saveSync('./trained.xml');
  }else {
    console.log('Not enough images uploaded yet', cvImages)
  }
}

function predictIt(fr, f){
  cv.readImage(f, function (err, im) {
    let result = fr.predictSync(im);
    console.log(`recognize result:(${f}) id=${result.id} conf=${100.0-result.confidence}`);
  });
}

//using defaults: .createLBPHFaceRecognizer(radius=1, neighbors=8, grid_x=8, grid_y=8, threshold=80)
const fr = new cv.FaceRecognizer();
trainIt(fr);
forEachFileInDir('./_bench', (f) => predictIt(fr, f));
```

## Test

Using [tape](https://github.com/substack/tape). Run with command:

`npm test`.

## Code coverage

Using [istanbul](http://gotwarlost.github.io/istanbul/) and [lcov](http://ltp.sourceforge.net/coverage/lcov.php). Run with command:

`make cover`

Build version of `opencv.node` will be generated, and coverage files will be put in `coverage/` directory. These files can be remvoved automatically by running `make clean`.

## MIT License
The library is distributed under the MIT License - if for some reason that
doesn't work for you please get in touch.
