var cv = require('../lib/opencv');


cv.readImage("./mona.png", function(err, im) {

				img_hsv = im.copy();
				img_gray = im.copy();

				img_hsv.convertHSVscale();
				img_gray.convertGrayscale();

				im.save("/tmp/nor.png");
				img_hsv.save("/tmp/hsv.png");
				img_gray.save("/tmp/gray.png");

				console.log("Guardado");
});

