var cv = require('../lib/opencv');

var COLOR = [0, 255, 0]; //default red
var thickness = 2; // default 1

cv.readImage('./mona.png', function(err, im) {

	im.detectObject('../data/haarcascade_frontalface_alt2.xml', {}, function(err, faces) {

		for(var k = 0; k < faces.length; k++) {

			face = faces[k];
			im.rectangle([face.x, face.y], [face.x + face.width, face.y + face.height], COLOR, 2);
		}

		im.save('/tmp/salida.png');

	});

});
