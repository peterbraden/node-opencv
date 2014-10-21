var cv = require('../lib/opencv');

var camera = new cv.VideoCapture(0);
var window = new cv.NamedWindow('Video', 0)

setInterval(function() {
  camera.read(function(err, im) {
    if (err) throw err;
    window.show(im);
    window.blockingWaitKey(0, 50);
  });
}, 20);
