var cv = require('../lib/opencv');


cv.readImage("./mona.png", function(err, im) {
	salt(im, 3000);
	im.save("/tmp/salt.png");
});



function salt(img, n) {


	if (img.channels() == 1) {

		console.log("1 Canales");
	} else if (img.channels() == 3) {

		for(k = 0; k < n; k ++) {
			i = Math.random() * img.width();
			j = Math.random() * img.height();

			img.set(j, i, 255);
		}
	}

}
