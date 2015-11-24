var cv = require('../lib/opencv');

// Load the image
cv.readImage('./files/note.png', function(err, im) {
  if (err) {
    throw err;
  }
  if (im.width() < 1 || im.height() < 1) {
    throw new Error('Image has no size');
  }

  im.cvtColor('CV_BGR2GRAY');
  var bw = im.adaptiveThreshold(255, 0, 0, 15, 2);
  bw.bitwiseNot(bw);

  var vertical = bw.clone();

  var verticalsize = vertical.size()[0] / 30;
  var verticalStructure = cv.imgproc.getStructuringElement(1, [1, verticalsize]);

  // Apply morphology operations
  vertical.erode(1, verticalStructure);
  vertical.dilate(1, verticalStructure);

  vertical.bitwiseNot(vertical);
  vertical.gaussianBlur([3, 3]);

  // Save output image
  vertical.save('./tmp/note.png');
});
