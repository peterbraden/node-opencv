var cv = require('../lib/opencv');

var vid = new cv.VideoCapture(0);

vid.read(function(err, mat) {
  if (err) throw err;

  var writer = new cv.VideoWriter('./tmp/output.mp4', 'MP4V', vid.getFPS(), mat.size(), true);
  writer.writeSync(mat);

  var x = 0;
  var iter = function () {
    vid.read(function (err, m2) {
      x++;
      writer.writeSync(m2);
      if (x < 100) {
        iter();
      } else {
        vid.release();
        writer.release();
      }
    })
  };
  iter();
});


