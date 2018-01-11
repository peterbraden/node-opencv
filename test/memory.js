require("v8").setFlagsFromString('--expose_gc');
var gc = require("vm").runInNewContext('gc');

var fs = require('fs')
  , path = require('path') 
  , test = require('tape')
  , cv = require('../lib/opencv');

 var IMAGE_PATH = path.resolve(__dirname, '../examples/files', 'mona.png');
 var VIDEO_PATH = path.resolve(__dirname, '../examples/files', 'motion.mov');
 var TEMP_SAVE_PATH = path.resolve(__dirname, '../examples/tmp', 'out.jpg');
 var TEMP_VIDEO_PATH = path.resolve(__dirname, '../examples/tmp', 'out.mp4');

// These tests check that every function that creates or modifies a Matrix handles its externally tracked memory correctly.
// Since the memory tracker uses OpenCV's reference counting to determine when to tell Node about memory changes,
// it is important that only Matrix objects that Javascript knows about retain references to internal OpenCV Mat objects.
// Reference counts for newly created objects should ususally therefore be 1, and releasing them should alter the 
// externally tracked memory appropriately.

// Note that garbage collection could run at any time, which could interfere with measurements of external memory,
// so these tests manually run garbage collection as part of their setup to minimize this possibility.

//********************
// Image Reading
//********************

test("readImage", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var image = cv.readImage(IMAGE_PATH);

	t.equal(image.getrefCount(), 1);
	t.equal(process.memoryUsage().external - startingMemory, 1134000); //image is tracked as external memory

	image.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

test("readImage creating a new 100x100 matrix", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var image = cv.readImage(100,100);

	t.equal(image.getrefCount(), 1);
	t.equal(process.memoryUsage().external - startingMemory, 80000); //image is tracked as external memory

	image.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

test("readImage from data buffer", t=>{
	gc();
	var buffer = fs.readFileSync(IMAGE_PATH);
	var startingMemory = process.memoryUsage().external;

	var image = cv.readImage(buffer);

	t.equal(image.getrefCount(), 1);
	t.equal(process.memoryUsage().external - startingMemory, 1134000); //image is tracked as external memory

	image.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

test("readImage (callback pattern)", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	cv.readImage(IMAGE_PATH, (err, image) => {
		t.equal(image.getrefCount(), 1);
		t.equal(process.memoryUsage().external - startingMemory, 1134000); //image is tracked as external memory

		image.release();

		var endingMemory = process.memoryUsage().external;
		t.equal(endingMemory - startingMemory, 0);
		t.end();
	});
});

test("readImage creating a new 100x100 matrix (callback pattern)", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	cv.readImage(100, 100, (err, image) => {
		t.equal(image.getrefCount(), 1);
		t.equal(process.memoryUsage().external - startingMemory, 80000); //image is tracked as external memory

		image.release();

		var endingMemory = process.memoryUsage().external;
		t.equal(endingMemory - startingMemory, 0);
		t.end();
	});
});

test("readImage from data buffer (callback pattern)", t=>{
	gc();
	var buffer = fs.readFileSync(IMAGE_PATH);
	var startingMemory = process.memoryUsage().external;

	cv.readImage(buffer, (err, image) => {
		t.equal(image.getrefCount(), 1);
		t.equal(process.memoryUsage().external - startingMemory, 1134000); //image is tracked as external memory

		image.release();

		var endingMemory = process.memoryUsage().external;
		t.equal(endingMemory - startingMemory, 0);
		t.end();
	});
});

test("readImageAsync", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	cv.readImageAsync(IMAGE_PATH, (err, image) => {
		t.equal(image.getrefCount(), 1);
		t.equal(process.memoryUsage().external - startingMemory, 1134000); //image is tracked as external memory

		image.release();

		var endingMemory = process.memoryUsage().external;
		t.equal(endingMemory - startingMemory, 0);
		t.end();
	});
});

test("readImageAsync creating a new 100x100 matrix", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	cv.readImageAsync(100, 100, (err, image) => {
		t.equal(image.getrefCount(), 1);
		t.equal(process.memoryUsage().external - startingMemory, 80000); //image is tracked as external memory

		image.release();

		var endingMemory = process.memoryUsage().external;
		t.equal(endingMemory - startingMemory, 0);
		t.end();
	});
});

test("readImageAsync from data buffer", t=>{
	gc();
	var buffer = fs.readFileSync(IMAGE_PATH);
	var startingMemory = process.memoryUsage().external;

	cv.readImageAsync(buffer, (err, image) => {
		t.equal(image.getrefCount(), 1);
		t.equal(process.memoryUsage().external - startingMemory, 1134000); //image is tracked as external memory

		image.release();

		var endingMemory = process.memoryUsage().external;
		t.equal(endingMemory - startingMemory, 0);
		t.end();
	});
});

test("video capture async", t=>{
	gc();
	var vid = new cv.VideoCapture(path.resolve(__dirname, '../examples/files', 'motion.mov'));

	var startingMemory = process.memoryUsage().external;

	vid.read( (err, im) => {

		t.equal(im.getrefCount(), 1);
		t.equal(process.memoryUsage().external - startingMemory, 545280); //image is tracked as external memory

		im.release();

		var endingMemory = process.memoryUsage().external;
		t.equal(endingMemory - startingMemory, 0);
		t.end();
	});
});

test("video capture sync", t=>{
	gc();
	var vid = new cv.VideoCapture(path.resolve(__dirname, '../examples/files', 'motion.mov'));

	var startingMemory = process.memoryUsage().external;

	var im = vid.readSync();

	t.equal(im.getrefCount(), 1);
	t.equal(process.memoryUsage().external - startingMemory, 545280);

	im.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

//********************
// Matrix Constructors
//********************

// Base constructor, doesn't actually allocate any memory
test("Matrix()", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var image = new cv.Matrix();

	t.equal(image.getrefCount(), -1);
	t.equal(process.memoryUsage().external - startingMemory, 0);

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

// Constructor with a size
test("Matrix(int, int)", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var image = new cv.Matrix(100, 100);

	t.equal(image.getrefCount(), 1);
	t.equal(process.memoryUsage().external - startingMemory, 120000); //100 * 100 * size of CV_32FC3 (12)

	image.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

// Constructor with a size and type
test("Matrix(int, int, type)", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var image = new cv.Matrix(100, 100, cv.Constants.CV_8UC1);

	t.equal(image.getrefCount(), 1);
	t.equal(process.memoryUsage().external - startingMemory, 10000); //100 * 100

	image.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

// Constructor with a size, type, and initial values
test("Matrix(int, int, type, [values])", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var image = new cv.Matrix(100, 100, cv.Constants.CV_8UC3, [0,0,0]);

	t.equal(image.getrefCount(), 1);
	t.equal(process.memoryUsage().external - startingMemory, 30000); //100 * 100 * 3

	image.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});


// Constructor with an existing matrix and a region of interest
test("Matrix(Matrix, x, y, w, h)", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var originalImage = new cv.Matrix(100, 100, cv.Constants.CV_8UC3, [0,0,0]);

	t.equal(originalImage.getrefCount(), 1);
	t.equal(process.memoryUsage().external - startingMemory, 30000); //100 * 100 * 3

	var image = new cv.Matrix(originalImage, 25, 25, 50, 50);        //this should share memory with the original
	t.equal(image.getrefCount(), 2);                                 //so the refcount goes up
	t.equal(process.memoryUsage().external - startingMemory, 30000); //but the memory usage does not

	originalImage.release();
	t.equal(originalImage.getrefCount(), -1);
	t.equal(image.getrefCount(), 1);
	t.equal(process.memoryUsage().external - startingMemory, 30000); 

	image.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

test("Matrix.Zeros", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var image = cv.Matrix.Zeros(100, 100);

	t.equal(image.getrefCount(), 1);
	t.equal(process.memoryUsage().external - startingMemory, 80000); //100 * 100 * size of CV_64FC1

	image.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

test("Matrix.Ones", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var image = cv.Matrix.Ones(100, 100);

	t.equal(image.getrefCount(), 1);
	t.equal(process.memoryUsage().external - startingMemory, 80000); //100 * 100 * size of CV_64FC1

	image.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

test("Matrix.Eye", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var image = cv.Matrix.Eye(100, 100);

	t.equal(image.getrefCount(), 1);
	t.equal(process.memoryUsage().external - startingMemory, 80000); //100 * 100 * size of CV_64FC1

	image.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

//********************
// Matrix Functions
//********************

test("Matrix clone", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var originalImage = new cv.Matrix(100, 100, cv.Constants.CV_8UC3, [0,0,0]);

	t.equal(originalImage.getrefCount(), 1);
	t.equal(process.memoryUsage().external - startingMemory, 30000); //100 * 100 * 3

	var image = originalImage.clone();

	t.equal(image.getrefCount(), 1);
	t.equal(process.memoryUsage().external - startingMemory, 60000);

	originalImage.release();
	t.equal(originalImage.getrefCount(), -1);
	t.equal(image.getrefCount(), 1);
	t.equal(process.memoryUsage().external - startingMemory, 30000); 

	image.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

test("Matrix crop", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var originalImage = new cv.Matrix(100, 100, cv.Constants.CV_8UC3, [0,0,0]);

	t.equal(originalImage.getrefCount(), 1);
	t.equal(process.memoryUsage().external - startingMemory, 30000); //100 * 100 * 3
	t.equal(originalImage.height(), 100);

	var image = originalImage.crop(25, 25, 50, 50); //crops share memory with the original
	t.equal(originalImage.height(), 100);
	t.equal(image.height(), 50);
	t.equal(image.getrefCount(), 2);
	t.equal(process.memoryUsage().external - startingMemory, 30000);

	originalImage.release();
	t.equal(originalImage.getrefCount(), -1);
	t.equal(image.getrefCount(), 1);
	t.equal(process.memoryUsage().external - startingMemory, 30000); 

	image.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

//ROI in this implementation is basically the same thing as crop
test("Matrix roi", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var originalImage = new cv.Matrix(100, 100, cv.Constants.CV_8UC3, [0,0,0]);

	t.equal(originalImage.getrefCount(), 1);
	t.equal(process.memoryUsage().external - startingMemory, 30000); //100 * 100 * 3
	t.equal(originalImage.height(), 100);

	var image = originalImage.roi(25, 25, 50, 50); //ROIs share memory with the original
	t.equal(originalImage.height(), 100);
	t.equal(image.height(), 50);
	t.equal(image.getrefCount(), 2);
	t.equal(process.memoryUsage().external - startingMemory, 30000);

	originalImage.release();
	t.equal(originalImage.getrefCount(), -1);
	t.equal(image.getrefCount(), 1);
	t.equal(process.memoryUsage().external - startingMemory, 30000); 

	image.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

test("Matrix convertGrayscale", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var originalImage = new cv.Matrix(100, 100, cv.Constants.CV_8UC3, [0,0,0]);
	t.equal(process.memoryUsage().external - startingMemory, 30000); //100 * 100 * 3

	originalImage.convertGrayscale();
	t.equal(process.memoryUsage().external - startingMemory, 10000); //grayscale takes less space
	t.equal(originalImage.getrefCount(), 1);

	originalImage.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

test("Matrix sobel", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var originalImage = new cv.Matrix(100, 100, cv.Constants.CV_8UC3, [0,0,0]);
	t.equal(process.memoryUsage().external - startingMemory, 30000); //100 * 100 * 3

	var resultImage = originalImage.sobel(cv.Constants.CV_16S, 1, 1);

	t.equal(process.memoryUsage().external - startingMemory, 90000); //our original 30k image plus our new 60k one
	t.equal(originalImage.getrefCount(), 1);
	t.equal(resultImage.getrefCount(), 1);

	originalImage.release();
	resultImage.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

test("Matrix copy", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var originalImage = new cv.Matrix(100, 100, cv.Constants.CV_8UC3, [0,0,0]);
	t.equal(process.memoryUsage().external - startingMemory, 30000); //100 * 100 * 3

	var resultImage = originalImage.copy(0);

	t.equal(process.memoryUsage().external - startingMemory, 60000); //our original image plus our new one
	t.equal(originalImage.getrefCount(), 1);
	t.equal(resultImage.getrefCount(), 1);

	originalImage.release();
	resultImage.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

test("Matrix flip", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var originalImage = new cv.Matrix(100, 100, cv.Constants.CV_8UC3, [0,0,0]);
	t.equal(process.memoryUsage().external - startingMemory, 30000); //100 * 100 * 3

	var resultImage = originalImage.flip(0);

	t.equal(process.memoryUsage().external - startingMemory, 60000); //our original image plus our new one
	t.equal(originalImage.getrefCount(), 1);
	t.equal(resultImage.getrefCount(), 1);

	originalImage.release();
	resultImage.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

test("Matrix dct", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var originalImage = new cv.Matrix(100, 100, cv.Constants.CV_32F);
	t.equal(process.memoryUsage().external - startingMemory, 40000); //100 * 100 * 4

	var resultImage = originalImage.dct();

	t.equal(process.memoryUsage().external - startingMemory, 80000); //our original image plus our new one
	t.equal(originalImage.getrefCount(), 1);
	t.equal(resultImage.getrefCount(), 1);

	originalImage.release();
	resultImage.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

test("Matrix idct", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var originalImage = new cv.Matrix(100, 100, cv.Constants.CV_32F);
	t.equal(process.memoryUsage().external - startingMemory, 40000); //100 * 100 * 4

	var resultImage = originalImage.idct();

	t.equal(process.memoryUsage().external - startingMemory, 80000); //our original image plus our new one
	t.equal(originalImage.getrefCount(), 1);
	t.equal(resultImage.getrefCount(), 1);

	originalImage.release();
	resultImage.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

test("Matrix add", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var src1 = new cv.Matrix.Ones(100, 100);
	var src2 = new cv.Matrix.Ones(100, 100);
	t.equal(process.memoryUsage().external - startingMemory, 160000);

	var resultMatrix = src1.add(src2);

	t.equal(resultMatrix.get(0,0), 2); //just making sure the result is correct

	t.equal(process.memoryUsage().external - startingMemory, 240000);
	t.equal(src1.getrefCount(), 1);
	t.equal(src2.getrefCount(), 1);
	t.equal(resultMatrix.getrefCount(), 1);

	src1.release();
	src2.release();
	resultMatrix.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

test("Matrix resize", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var image = new cv.Matrix(100, 100, cv.Constants.CV_8UC3, [0,0,0]);
	t.equal(process.memoryUsage().external - startingMemory, 30000); //100 * 100 * 3

	image.resize(50, 50);
	t.equal(process.memoryUsage().external - startingMemory, 7500); //50 * 50 * 3

	image.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

test("Matrix resize async", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var image = new cv.Matrix(100, 100, cv.Constants.CV_8UC3, [0,0,0]);
	t.equal(process.memoryUsage().external - startingMemory, 30000); //100 * 100 * 3

	image.resize(50, 50, (err, resizedImage)=>{
		t.equal(image.height(), 100);      //we have both the original and the resized image
		t.equal(resizedImage.height(), 50);

		t.equal(image.getrefCount(), 1);
		t.equal(resizedImage.getrefCount(), 1);


		t.equal(process.memoryUsage().external - startingMemory, 37500);

		image.release();
		resizedImage.release();

		var endingMemory = process.memoryUsage().external;
		t.equal(endingMemory - startingMemory, 0);
		t.end();
	});	
});

test("Matrix resize async edge case", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var image = new cv.Matrix(1000, 1000, cv.Constants.CV_8UC3, [0,0,0]);
	t.equal(process.memoryUsage().external - startingMemory, 3000000); //100 * 100 * 3

	image.resize(50, 50, (err, resizedImage)=>{
		t.equal(image.getrefCount(), -1);  //this happens second, image should have been released already
		t.equal(resizedImage.getrefCount(), 1);

		t.equal(process.memoryUsage().external - startingMemory, 7500);

		resizedImage.release();

		var endingMemory = process.memoryUsage().external;
		t.equal(endingMemory - startingMemory, 0);
		t.end();
	});
	image.release(); //this happens first
});

test("Matrix threshold", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var originalImage = new cv.Matrix(100, 100, cv.Constants.CV_8U);
	t.equal(process.memoryUsage().external - startingMemory, 10000); //100 * 100

	var resultImage = originalImage.threshold(1,1);

	t.equal(process.memoryUsage().external - startingMemory, 20000); //our original image plus our new one
	t.equal(originalImage.getrefCount(), 1);
	t.equal(resultImage.getrefCount(), 1);

	originalImage.release();
	resultImage.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

test("Matrix adaptiveThreshold", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var originalImage = new cv.Matrix(100, 100, cv.Constants.CV_8U);
	t.equal(process.memoryUsage().external - startingMemory, 10000); //100 * 100

	var resultImage = originalImage.adaptiveThreshold(255, 0, 0, 15, 2);

	t.equal(process.memoryUsage().external - startingMemory, 20000); //our original image plus our new one
	t.equal(originalImage.getrefCount(), 1);
	t.equal(resultImage.getrefCount(), 1);

	originalImage.release();
	resultImage.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

test("Matrix meanStdDev", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var matrix = new cv.Matrix.Ones(100, 100, cv.Constants.CV_8UC3);
	t.equal(process.memoryUsage().external - startingMemory, 30000);

	var result = matrix.meanStdDev();

	t.equal(result.mean.getrefCount(), 1);
	t.equal(result.stddev.getrefCount(), 1);
	t.equal(process.memoryUsage().external - startingMemory, 30048);

	matrix.release();
	result.mean.release();
	result.stddev.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

test("Matrix copyTo", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var smallImg = new cv.Matrix.Ones(25, 25, cv.Constants.CV_8UC3);
	var bigImg = cv.Matrix.Zeros(100, 100, cv.Constants.CV_8UC3);
	t.equal(process.memoryUsage().external - startingMemory, 30000 + 1875);

	smallImg.copyTo(bigImg, 0, 0);

	t.equal(smallImg.getrefCount(), 1);
	t.equal(bigImg.getrefCount(), 1);
	t.equal(process.memoryUsage().external - startingMemory, 30000 + 1875);

	smallImg.release();
	bigImg.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

test("Matrix cvtColor", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var image = new cv.Matrix(100, 100, cv.Constants.CV_8UC3, [0,0,0]);
	t.equal(process.memoryUsage().external - startingMemory, 30000); //100 * 100 * 3

	image.cvtColor("CV_BGR2GRAY");
	t.equal(process.memoryUsage().external - startingMemory, 10000); //grayscale is smaller

	image.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

test("Matrix split", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var image = new cv.Matrix(100, 100, cv.Constants.CV_8UC3, [0,0,0]);
	t.equal(process.memoryUsage().external - startingMemory, 30000); //100 * 100 * 3

	var result = image.split();
	t.equal(process.memoryUsage().external - startingMemory, 60000);

	image.release();
	t.equal(process.memoryUsage().external - startingMemory, 30000);

	result.forEach(m => m.release());

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

test("Matrix merge", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var image1 = new cv.Matrix(100, 100, cv.Constants.CV_8UC1, [0]);
	var image2 = new cv.Matrix(100, 100, cv.Constants.CV_8UC1, [0]);
	var image3 = new cv.Matrix(100, 100, cv.Constants.CV_8UC1, [0]);
	t.equal(process.memoryUsage().external - startingMemory, 30000); //100 * 100 * 3

	var result = new cv.Matrix(10,10);
	t.equal(process.memoryUsage().external - startingMemory, 30000 + 1200);

	result.merge([image1, image2, image3]);

	t.equal(process.memoryUsage().external - startingMemory, 60000);

	result.release();
	image1.release();
	image2.release();
	image3.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

test("Matrix reshape", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var image = new cv.Matrix(100, 100, cv.Constants.CV_8UC3, [0,0,0]);
	t.equal(process.memoryUsage().external - startingMemory, 30000); //100 * 100 * 3

	var result = image.reshape(2);

	t.equal(process.memoryUsage().external - startingMemory, 30000); //reshape does not copy data, so the allocated size hasn't changed

	image.release();

	t.equal(process.memoryUsage().external - startingMemory, 30000);

	result.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);
	t.end();
});

//********************
// Additional Asynchronous Matrix Functions
//********************


test("Matrix toBuffer async edge case", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var image = new cv.Matrix(1000, 1000, cv.Constants.CV_8UC3, [0,0,0]);
	t.equal(process.memoryUsage().external - startingMemory, 3000000); //100 * 100 * 3

	image.toBufferAsync((err, buffer)=>{
		t.equal(image.getrefCount(), -1);  //this happens second, image should have been released already

		var endingMemory = process.memoryUsage().external;
		t.equal(endingMemory - startingMemory, 33006); //the size of the buffer, which hasn't been released yet
		t.end();
	});
	image.release();
});

test("Matrix save async edge case", t=>{
	gc();
	var startingMemory = process.memoryUsage().external;

	var image = new cv.Matrix(1000, 1000, cv.Constants.CV_8UC3, [0,0,0]);
	t.equal(process.memoryUsage().external - startingMemory, 3000000); //100 * 100 * 3

	image.saveAsync(TEMP_SAVE_PATH, (err, buffer)=>{
		t.equal(image.getrefCount(), -1);  //this happens second, image should have been released already

		var endingMemory = process.memoryUsage().external;
		t.equal(endingMemory - startingMemory, 0);

		fs.unlinkSync(TEMP_SAVE_PATH);
		t.end();
	});
	image.release();
});

//********************
// Background Subtractors
//********************

function testSyncBackgroundSubtractor(t, subtractor){
	gc();
	var startingMemory = process.memoryUsage().external;

	var image = new cv.Matrix(1000, 1000, cv.Constants.CV_8UC3, [0,0,0]);
	t.equal(process.memoryUsage().external - startingMemory, 3000000); //100 * 100 * 3

	var output = subtractor.apply(image);
	t.equal(image.getrefCount(), 1);
	t.equal(output.getrefCount(), 1);

	image.release();
	output.release();

	var endingMemory = process.memoryUsage().external;
	t.equal(endingMemory - startingMemory, 0);

	t.end();
}

function testAsyncBackgroundSubtractor(t, subtractor){
	gc();
	var startingMemory = process.memoryUsage().external;

	var image = new cv.Matrix(1000, 1000, cv.Constants.CV_8UC3, [0,0,0]);
	t.equal(process.memoryUsage().external - startingMemory, 3000000); //100 * 100 * 3

	subtractor.apply(image, (err, output) => {
		t.equal(image.getrefCount(), 1);
		t.equal(output.getrefCount(), 1);

		image.release();
		output.release();

		var endingMemory = process.memoryUsage().external;
		t.equal(endingMemory - startingMemory, 0);

		t.end();
	});
}

function testAsyncBackgroundSubtractorEarlyRelease(t, subtractor){
	gc();
	var startingMemory = process.memoryUsage().external;

	var image = new cv.Matrix(1000, 1000, cv.Constants.CV_8UC3, [0,0,0]);
	t.equal(process.memoryUsage().external - startingMemory, 3000000); //100 * 100 * 3

	subtractor.apply(image, (err, output) => {
		t.equal(image.getrefCount(), -1);
		t.equal(output.getrefCount(), 1);

		output.release();

		var endingMemory = process.memoryUsage().external;
		t.equal(endingMemory - startingMemory, 0);

		t.end();
	});

	image.release();
}

test("default background subtractor", t=>{
	testSyncBackgroundSubtractor(t, new cv.BackgroundSubtractor());
});

test("MOG background subtractor", t=>{
	testSyncBackgroundSubtractor(t, cv.BackgroundSubtractor.createMOG());
});

test("MOG2 background subtractor", t=>{
	testSyncBackgroundSubtractor(t, cv.BackgroundSubtractor.createMOG2());
});

test("GMG background subtractor", t=>{
	testSyncBackgroundSubtractor(t, cv.BackgroundSubtractor.createGMG());
});

test("default background subtractor async", t=>{
	testAsyncBackgroundSubtractor(t, new cv.BackgroundSubtractor());
});

test("MOG background subtractor async", t=>{
	testAsyncBackgroundSubtractor(t, cv.BackgroundSubtractor.createMOG());
});

test("MOG2 background subtractor async", t=>{
	testAsyncBackgroundSubtractor(t, cv.BackgroundSubtractor.createMOG2());
});

test("GMG background subtractor async", t=>{
	testAsyncBackgroundSubtractor(t, cv.BackgroundSubtractor.createGMG());
});

test("default background subtractor async early release", t=>{
	testAsyncBackgroundSubtractorEarlyRelease(t, new cv.BackgroundSubtractor());
});

test("MOG background subtractor async early release", t=>{
	testAsyncBackgroundSubtractorEarlyRelease(t, cv.BackgroundSubtractor.createMOG());
});

test("MOG2 background subtractor async early release", t=>{
	testAsyncBackgroundSubtractorEarlyRelease(t, cv.BackgroundSubtractor.createMOG2());
});

test("GMG background subtractor async early release", t=>{
	testAsyncBackgroundSubtractorEarlyRelease(t, cv.BackgroundSubtractor.createGMG());
});

//********************
// cascade classifier
//********************

test("cascade classifier async", t=> {
	gc();
	var startingMemory = process.memoryUsage().external;

	var image = new cv.readImage(path.resolve(__dirname, '../examples/files', 'mona.png'));

	t.equal(process.memoryUsage().external - startingMemory, 1134000); //100 * 100 * 3

	var classifier = new cv.CascadeClassifier(cv.FACE_CASCADE);

	classifier.detectMultiScale(image, (err, faces) =>{
		t.equal(image.getrefCount(), 1);
		image.release();

		var endingMemory = process.memoryUsage().external;
		t.equal(endingMemory - startingMemory, 0);

		t.end();
	});
});

test("cascade classifier async early release", t=> {
	gc();
	var startingMemory = process.memoryUsage().external;

	var image = new cv.readImage(path.resolve(__dirname, '../examples/files', 'mona.png'));

	t.equal(process.memoryUsage().external - startingMemory, 1134000);

	var classifier = new cv.CascadeClassifier(cv.FACE_CASCADE);

	classifier.detectMultiScale(image, (err, faces) =>{
		t.equal(image.getrefCount(), -1);

		var endingMemory = process.memoryUsage().external;
		t.equal(endingMemory - startingMemory, 0);

		t.end();
	});
	image.release();
});

//********************
// VideoWriter
//********************


test("Video writer async edge case", t=>{
	gc();

	var startingMemory = process.memoryUsage().external;

	var reader = new cv.VideoCapture(VIDEO_PATH);

	reader.read((err, image)=>{
		t.equal(process.memoryUsage().external - startingMemory, 545280);
		var writer = new cv.VideoWriter(TEMP_VIDEO_PATH, 'mp4v', 1, image.size(), true);

		writer.write(image, err=>{
			t.equal(image.getrefCount(), -1);  //this happens second, image should have been released already
			var endingMemory = process.memoryUsage().external;
			t.equal(endingMemory - startingMemory, 0);

			writer.release();
			fs.unlinkSync(TEMP_VIDEO_PATH);
			t.end();
		});

		image.release();
	});
});

//********************
// Face Recognizer
//********************
