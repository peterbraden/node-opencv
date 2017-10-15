var cv = require('../lib/opencv');

var COLOR = [0, 255, 0]; // default red
var thickness = 2; // default 1

cv.readImage('./files/mona.png', function(err, im) {
  if (err) throw err;
  if (im.width() < 1 || im.height() < 1) throw new Error('Image has no size');

  im.detectObject('../data/haarcascade_frontalface_alt2.xml', {}, function(err, faces) {
    if (err) throw err;

    for (var i = 0; i < faces.length; i++) {
      var face = faces[i];
      im.rectangle([face.x, face.y], [face.width, face.height], COLOR, 2);
    }

    im.save('./tmp/face-detection-rectangle.png');
    console.log('Image saved to ./tmp/face-detection-rectangle.png');
  });

});
