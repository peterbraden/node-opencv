var cv = require('../lib/opencv');

cv.readImage('./examples/files/mona.png', function(err, im) {
  if (err) throw err;
  if (im.width() < 1 || im.height() < 1) throw new Error('Image has no size');

  img_hsv = im.copy();
  img_gray = im.copy();

  img_hsv.convertHSVscale();
  img_gray.convertGrayscale();

  im.save('./examples/tmp/nor.png');
  img_hsv.save('./examples/tmp/hsv.png');
  img_gray.save('./examples/tmp/gray.png');

  img_crop = im.crop(50,50,250,250);
  img_crop.save('./examples/tmp/crop.png');

  console.log('Image saved to ./examples/tmp/{crop|nor|hsv|gray}.png');
});
