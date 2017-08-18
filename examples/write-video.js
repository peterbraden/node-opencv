var cv = require('../lib/opencv');
var path = require('path');

var vid = new cv.VideoCapture(path.join(__dirname, 'files', 'motion.mov'));

vid.read(function(err, mat) {
  if (err) throw err;

  var filename = './tmp/output-'+new Date().getTime()+'.mov';
  var writer = new cv.VideoWriter(filename, 'X264', vid.getFPS(), mat.size(), true);
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


