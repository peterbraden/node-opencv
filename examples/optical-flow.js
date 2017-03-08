var cv = require('../lib/opencv');
var path = require('path');

var cap = new cv.VideoCapture(path.join(__dirname, 'files', 'motion.mov'));
var window = new cv.NamedWindow('Video', 0);

// Parameters for lucas kanade optical flow
var lk_params = {
  winSize: [15, 15],
  maxLevel: 2,
  criteria: [cv.TERM_CRITERIA_EPS | cv.TERM_CRITERIA_COUNT, 30, 0.03]
};

feature_params = {
  maxCorners: 100,
  qualityLevel: 0.1,
  minDistance: 10
};

// Create some random colors
var color = [255, 0, 0];

// Take first frame and find corners in it
cap.read(function(err, firstFrame) {
  if (err) throw err;

  var old_frame = firstFrame;

  // Create a mask image for drawing purposes
  function read() {
    var out = old_frame.copy();
    cap.read(function(err, newFrame) {
      if (err) throw err;

      var goodFeatures = old_frame.goodFeaturesToTrack(feature_params.maxCorners, feature_params.qualityLevel, feature_params.minDistance);

      // calculate optical flow
      var flow = old_frame.calcOpticalFlowPyrLK(newFrame, goodFeatures, lk_params.winSize, lk_params.maxLevel, lk_params.criteria);

      // Select good points

      // draw the tracks
      for(var i = 0; i < flow.old_points.length; i++){
        if(flow.found[i]){
          out.line(flow.old_points[i], flow.new_points[i], color);
        }
      }
      window.show(out);
      window.blockingWaitKey(0, 50);
      old_frame = newFrame.copy();
      read();
    });
  }

  read();
});