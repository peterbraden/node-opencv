var cv = require('../lib/opencv');

cv.readImage("./files/mona.png", function(err, orig) {
  if (err) throw err;

  var chan1 = orig.split()[0];

  var floatInputMatrix = new cv.Matrix();
  chan1.convertTo(floatInputMatrix, cv.Constants.CV_32F)

  var foatAfterDct = floatInputMatrix.dct();

  // for inverse dct, set argument to "true"
  var afterDoubleDct = foatAfterDct.dct(true);

  var intOutImage = new cv.Matrix();

  afterDoubleDct.convertTo(intOutImage, cv.Constants.CV_8U);
  intOutImage.save("./tmp/dct.png");
  console.log('Image saved to ./tmp/dct.png');

});
