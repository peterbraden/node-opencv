var cv = require('../lib/opencv');


cv.readImage("./mona.png", function(err, orig) {

	cv.readImage("./over_text.png", function(err, over_text) {

		var result = new cv.Matrix(orig.width(), orig.height());

		result.addWeighted(orig, 0.7, over_text, 0.9);
		result.save("/tmp/weighted.png");
	});

});


