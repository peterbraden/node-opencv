var cv = require('../lib/opencv');

//
//  Example of use of EMD distance using histograms
//  1. Build 2 histograms from images using calcHist
//  2. Transform each histogram to a 64 x 4 (hist, b, g, r) x 1 normalized signatures in BGR space
//  3. Compute the cost matrix (64 x 64 x 1), calculating the cost in LUV space
//  4. Run EMD algorithm
//

/// Useful flatten function for step 2

function flatten(array, accu) {
  if(!accu){
    accu = [];
  }
  array.forEach(function(a){
    if(Array.isArray(a)) {
      flatten(a, accu)
    } else {
      accu.push(a)
    }
  });
  return accu
};

cv.readImage('./files/car1.jpg', function(err, im1) {
  if (err) throw err;
  if (im1.width() < 1 || im1.height() < 1) throw new Error('Image has no size');
  cv.readImage('./files/car2.jpg', function(err, im2) {
    if (err) throw err;
    if (im2.width() < 1 || im2.height() < 1) throw new Error('Image has no size');

    ///////////////////
    //  1. Build 2 histograms from images using calcHist
    //////////////////

    var size = [4, 4, 4],
        channels = [0, 1, 2],
        range = [[0, 256], [0, 256], [0, 256]],
        uniform = true,
        accumulate = true,
        histFile = 'files/chart2.png';

    /// Compute 64 (=4^3) histograms:
    var firstImageHist64 = cv.histogram.calcHist(im1, channels, size, range, uniform);
    var secondImageHist64 = cv.histogram.calcHist(im2, channels, size, range, uniform);

    //////////////
    //  2. Transform each histogram to a 64 x 4 (hist, b, g, r) x 1 normalized signatures in BGR space
    ////////////////

    var step = 256/4;
    var halfStep = Math.round(step/2);

    var sum1 = 0;
    var sum2 = 0;

    firstImageHist64.map(function(bHist, bIndex){
      return bHist.map(function(bgHist, gIndex){
        return bgHist.map(function(bgrHist, rIndex){
          sum1 += bgrHist;
        })
      })
    })

    var sig1 = flatten(firstImageHist64.map(function(bHist, bIndex){
      return bHist.map(function(bgHist, gIndex){
        return bgHist.map(function(bgrHist, rIndex){
          return {
            data :[
              [bgrHist/sum1],
              [bIndex*step + halfStep],
              [gIndex*step + halfStep],
              [rIndex*step + halfStep]
            ]
          }
        })
      })
    })).map(function(a){
      // trick to avoid flattening and get a 64 x 4 x 1 image as needed
      return a.data;
    });

    secondImageHist64.map(function(bHist, bIndex){
      return bHist.map(function(bgHist, gIndex){
        return bgHist.map(function(bgrHist, rIndex){
          sum2 += bgrHist;
        })
      })
    })

    var sig2 = flatten(secondImageHist64.map(function(bHist, bIndex){
      return bHist.map(function(bgHist, gIndex){
        return bgHist.map(function(bgrHist, rIndex){
          return {
            data : [
              [bgrHist/sum2],
              [bIndex*step + halfStep],
              [gIndex*step + halfStep],
              [rIndex*step + halfStep]
            ]
          };
        })
      })
    })).map(function(a){
      // trick to avoid flattening and get a 64 x 4 x 1 image as needed
      return a.data;
    });

    /////////////
    //  3. Compute the cost matrix (64 x 64 x 1), calculating the cost in LUV space
    /////////////

    //middles is a 1 x 64 x 3 array of the middles positions in RGB used to change to LUV
    var middles = [flatten(firstImageHist64.map(function(bHist, bIndex){
      return bHist.map(function(bgHist, gIndex){
        return bgHist.map(function(bgrHist, rIndex){
          return {
            data : [
              bIndex*step + halfStep,
              gIndex*step + halfStep,
              rIndex*step + halfStep
            ]
          }
        })
      })
    })).map(function(a){
      // trick to avoid flattening and get a 1 x 64 x 3 image as needed
      return a.data;
    })];

    var mat = cv.Matrix.fromArray(middles, cv.Constants.CV_8UC3);
    mat.cvtColor("CV_BGR2Luv");

    //luvValues is a 1 x 64 x 3 array of the middles positions in LUV
    var luvMiddles = mat.toArray();

    var distance = function(luv1, luv2){
      return Math.sqrt((luv1[0]-luv2[0])*(luv1[0]-luv2[0]) + (luv1[1]-luv2[1])*(luv1[1]-luv2[1]) + (luv1[2]-luv2[2])*(luv1[2]-luv2[2]));
    };

    var costs = luvMiddles[0].map(function(luvMiddle1){
      return luvMiddles[0].map(function(luvMiddle2){
        return [distance(luvMiddle1, luvMiddle2)];
      })
    });

    //////
    //  4. Run EMD algorithm
    /////

    var matCosts = cv.Matrix.fromArray(costs, cv.Constants.CV_32FC1);
    var matSig1 = cv.Matrix.fromArray(sig1, cv.Constants.CV_32FC1);
    var matSig2 = cv.Matrix.fromArray(sig2, cv.Constants.CV_32FC1);

    var dist = cv.Constants.CV_DIST_L2;
    var emd = cv.histogram.emd(matSig1, matSig2, dist);//, matCosts);
    console.log("EMD is ", emd)
  });
});
