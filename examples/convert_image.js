var cv = require('../lib/opencv');


cv.readImage("./mona.png", function(err, im) {

				img_hsv = im.copy();
				img_gray = im.copy();


				img_hsv.convertHSVscale();
				img_gray.convertGrayscale();

				console.log(img_gray.pixel(100,100));

				im.save("/tmp/nor.png");
				img_hsv.save("/tmp/hsv.png");
				img_gray.save("/tmp/gray.png");

				img_crop = im.crop(50,50,250,250);
				img_crop.save("crop.png");

				console.log("Guardado");
});

