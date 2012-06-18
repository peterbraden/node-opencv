var cv = require('../lib/opencv');

var camera = new cv.VideoCapture(0);


setInterval(function() {

	camera.read(function(im) {

		im.save('/tmp/cam.png');
	});

}, 1000);
