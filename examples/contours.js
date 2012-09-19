var cv = require('../lib/opencv');

var lowThresh = 0;
var highThresh = 100;
var nIters = 2;
var maxArea = 2500;

var GREEN = [0, 255, 0]; //B, G, R
var WHITE = [255, 255, 255]; //B, G, R


cv.readImage('./stuff.png', function(err, im) {

	var big = new cv.Matrix(im.width(), im.height()); 
	var all = new cv.Matrix(im.width(), im.height()); 

	im.convertGrayscale();
	im_canny = im.copy();

	im_canny.canny(lowThresh, highThresh);
	im_canny.dilate(nIters);

	contours = im_canny.findContours();

	for(i = 0; i < contours.size(); i++) {
		if(contours.area(i) > maxArea) {
			big.drawContour(contours, i, GREEN);
		}
	}

	all.drawAllContours(contours, WHITE);


	big.save('./big.png');
	all.save('./all.png');
});
