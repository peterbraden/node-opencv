var cv = require('../lib/opencv');

// (B)lue, (G)reen, (R)ed
var histSize = 256;

cv.readImage('./files/car1.jpg', function(err, im) {
  if (err) throw err;
  if (im.width() < 1 || im.height() < 1) throw new Error('Image has no size');

  var bgrPlanes = im.split();

  var size = 4,
      range = [0, 256],
      uniform = true,
      accumulate = true,
      histFile = 'files/chart.png';


  /// Compute the histograms:
  var bHist = cv.histogram.calcHist( bgrPlanes[0], size, range, uniform, accumulate );
  var gHist = cv.histogram.calcHist( bgrPlanes[1], size, range, uniform, accumulate );
  var rHist = cv.histogram.calcHist( bgrPlanes[2], size, range, uniform, accumulate );

  console.log("Histograms are \n Blue : ", bHist, "\n Green : ", gHist, "\n Red : ", rHist);


});
