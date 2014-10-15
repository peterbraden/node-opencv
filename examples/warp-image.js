var cv = require('../lib/opencv');

cv.readImage("./examples/files/mona.png", function(err, im) {
  if (err) throw err;

  var width = im.width();
  var height = im.height();
  if (width < 1 || height < 1) throw new Error('Image has no size');

  var srcArray = [0, 0, width, 0, width, height, 0, height];
  var dstArray = [0, 0, width * 0.9, height * 0.1, width, height, width * 0.2, height * 0.8];
  var xfrmMat = im.getPerspectiveTransform(srcArray, dstArray);
  im.warpPerspective(xfrmMat, width, height, [255, 255, 255]);
  im.save("./examples/tmp/warp-image.png");
  console.log('Image saved to ./tmp/warp-image.png');
});
