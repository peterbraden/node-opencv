var path = require('path'),
    cv = require('../lib/opencv');

// When opening a file, the full path must be passed to opencv
var vid = new cv.VideoCapture(path.join(__dirname, "motion.mov"));

vid.read(function(err, mat){
  var track = new cv.TrackedObject(mat, [420, 110, 490, 170], {channel: "value"});
  var x = 0;
  var iter = function(){
    vid.read(function(err, m2){
      x++;
      var rec = track.track(m2)
      console.log(">>", x, ":" , rec)
      if (x % 10 == 0){
        m2.rectangle([rec[0], rec[1]], [rec[2], rec[3]])
 //       m2.save('./out-motiontrack-' + x + '.jpg')
      }
      if (x<100)
        iter();
    })
  }
  iter();
})

