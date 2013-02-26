#!/usr/bin/env node

//
// Finds quadrilaterals and fills them with an X
//

var cv = require('../lib/opencv');

var lowThresh = 0;
var highThresh = 100;
var nIters = 2;
var minArea = 2000;
var maxArea = 100000;

var BLUE = [0, 255, 0]; //B, G, R
var RED   = [0, 0, 255]; //B, G, R
var GREEN = [0, 255, 0]; //B, G, R
var WHITE = [255, 255, 255]; //B, G, R


cv.readImage('./quads.jpg', function(err, im) {

	var out = im.copy();

	im.convertGrayscale();
	im_canny = im.copy();

	im_canny.canny(lowThresh, highThresh);
	im_canny.dilate(nIters);

	contours = im_canny.findContours();

	for(i = 0; i < contours.size(); i++) {

		var area = contours.area(i);
		if(area < minArea || area > maxArea) continue;

		var arcLength = contours.arcLength(i, true);
		contours.approxPolyDP(i, 0.01 * arcLength, true);

		if(contours.cornerCount(i) != 4) continue;

		var points = [
			contours.point(i, 0),
			contours.point(i, 1),
			contours.point(i, 2),
			contours.point(i, 3)
		]

		out.line([points[0].x,points[0].y], [points[2].x, points[2].y], RED);
		out.line([points[1].x,points[1].y], [points[3].x, points[3].y], RED);
	}

	out.save('./out.png');
});
