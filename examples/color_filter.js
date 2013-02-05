var cv = require('../lib/opencv');

// (B)lue, (G)reen, (R)ed
var lower_threshold = [46, 57, 83];
var upper_threshold = [80, 96, 115];

cv.readImage('./coin1.jpg', function(err, im) {

	im.inRange(lower_threshold, upper_threshold);
	im.save('./coin_detected.jpg');

});
