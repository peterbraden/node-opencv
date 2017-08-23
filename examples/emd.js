var cv = require('../lib/opencv');

// (B)lue, (G)reen, (R)ed
var histSize = 256;

cv.readImage('./files/car1.jpg', function(err, im1) {
  if (err) throw err;
  if (im.width() < 1 || im.height() < 1) throw new Error('Image has no size');
  cv.readImage('./files/car2.jpg', function(err, im2) {

    var bgrPlanes = im.split();

    var size = [4, 4, 4],
        channels = [0, 1, 2],
        range = [[0, 256], [0, 256], [0, 256]],
        uniform = true,
        accumulate = true,
        histFile = 'files/chart2.png';


    /// Compute 64 (=4^3) histograms:

    var firstImageHist64 = cv.histogram.calcHist(im1, channels, size, range, uniform, accumulate);
    var secondImageHist64 = cv.histogram.calcHist(im2, channels, size, range, uniform, accumulate);

    var sig1 = flatten(firstImageHist64);
    var sig2 = flatten(secondImageHist64);

    var step = 256/4;
    var halfStep = Math.round(step/2)

    var middles = flatten(firstImageHist64.map(function(bHist, bIndex){
      return blueValue.map(function(bgHist, gIndex){
        return greenValue.map(function(bgrHist, rIndex){
          return [
            bIndex*step + halfStep,
            gIndex*step + halfStep,
            rIndex*step + halfStep
          ];/*{
            b : bIndex*step + halfStep,
            g : gIndex*step + halfStep,
            r : rIndex*step + halfStep
          }*/
        })
      })
    }));

    var mat = new opencv.Matrix(64,1,cv.Constants.CV_8UC3);
    var buf = Buffer(64*1*3);
    buf.fill(0);
    for(var i=0;i<64*1*3;i++){
      buf[i] = middles[i];
    }
    mat.put(buf);
    mat.cvtColor("CV_BGR2Luv");

    var luvValues = mat.toArray()[0];

    var distance = function(luv1, luv2){
      return Math.sqrt((luv1[0]-luv2[0])*(luv1[0]-luv2[0]) + (luv1[1]-luv2[1])*(luv1[1]-luv2[1]) + (luv1[2]-luv2[2])*(luv1[2]-luv2[2]));
    };

    var costs = luvValues.map(function(luvMiddle1){
      return middles.map(function(luvMiddle2){
        return distance(luvMiddle1, luvMiddle2);
      })
    })

    emd(sig1, sig2, cv.Constants.CV_DIST_USER, cost);

  });
});
