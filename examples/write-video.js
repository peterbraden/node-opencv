var cv = require('../lib/opencv');
var path = require('path');

var vid = new cv.VideoCapture(path.join(__dirname, 'files', 'motion.mov'));

vid.read(function(err, mat) {
  if (err) throw err;

  var filename = './output-'+new Date().getTime()+'.avi';
  var writer = new cv.VideoWriter(filename, 'DIVX', vid.getFPS(), mat.size(), true);
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



// restart video read
var vid2 = new cv.VideoCapture(path.join(__dirname, 'files', 'motion.mov'));

var filename2 = './output-async-'+new Date().getTime()+'.avi';
var writer2 = null;
var x = 0;

// do the same write async
var iter = function () {
  vid2.read(function (err, m2) {
	if (writer2 === null)
	  writer2 = new cv.VideoWriter(filename2, 'DIVX', vid2.getFPS(), m2.size(), true);

	x++;
	writer2.write(m2, function(err){
	  if (x < 100) {
		iter();
	  } else {
		vid2.release();
		writer2.release();
	  }
	});
	m2.release();
	delete m2;
  });
};

// kick it off
iter();
