var cv = require('../lib/opencv');

// (B)lue, (G)reen, (R)ed
var lower_threshold = [46, 57, 83];
var upper_threshold = [80, 96, 115];

cv.readImage('./examples/files/coin1.jpg', function(err, im) {
  if (err) throw err;
  if (im.width() < 1 || im.height() < 1) throw new Error('Image has no size');

  im.inRange(lower_threshold, upper_threshold);
  im.save('./examples/tmp/coin_detected.jpg');
  console.log('Image saved to ./tmp/coin_detected.jpg');
});
