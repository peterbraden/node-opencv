#!/usr/bin/env node

//
// Detects triangles and quadrilaterals
//

var cv = require('../lib/opencv');

var lowThresh = 0;
var highThresh = 100;
var nIters = 2;
var minArea = 2000;

var BLUE = [0, 255, 0]; //B, G, R
var RED   = [0, 0, 255]; //B, G, R
var GREEN = [0, 255, 0]; //B, G, R
var WHITE = [255, 255, 255]; //B, G, R


cv.readImage('./shapes.jpg', function(err, im) {

	var out = new cv.Matrix(im.height(), im.width());

	im.convertGrayscale();
	im_canny = im.copy();

	im_canny.canny(lowThresh, highThresh);
	im_canny.dilate(nIters);

	contours = im_canny.findContours();

	for(i = 0; i < contours.size(); i++) {

		if(contours.area(i) < minArea) continue;

		var arcLength = contours.arcLength(i, true);
		contours.approxPolyDP(i, 0.01 * arcLength, true);

		switch(contours.cornerCount(i)) {
		case 3:
			out.drawContour(contours, i, GREEN);
			break;
		case 4:
			out.drawContour(contours, i, RED);
			break;
		default:
			out.drawContour(contours, i, WHITE);
		}
	}

	out.save('./out.png');
});
