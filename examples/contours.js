var cv = require('../lib/opencv');

var lowThresh = 0;
var highThresh = 100;
var nIters = 2;
var maxArea = 2500;

var GREEN = [0, 255, 0]; //B, G, R
var WHITE = [255, 255, 255]; //B, G, R
var RED   = [0, 0, 255]; //B, G, R

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
			var moments = contours.moments(i);
			var cgx = Math.round(moments.m10/moments.m00);
			var cgy = Math.round(moments.m01/moments.m00);
			big.drawContour(contours, i, GREEN);
			big.line([cgx - 5, cgy], [cgx + 5, cgy], RED);
			big.line([cgx, cgy - 5], [cgx, cgy + 5], RED);
		}
	}

	all.drawAllContours(contours, WHITE);


	big.save('./big.png');
	all.save('./all.png');
});
