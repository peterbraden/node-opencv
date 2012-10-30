  var buf, cv;

cv = require('../lib/opencv');
var fs = require('fs')

buf = fs.readFileSync('../examples/stuff.png')

var cb = function(){console.log("OK")}

cv.readImage(buf, function(err, im) {
  if (err != null) {
    return cb(err);
  }
  return im.detectObject("../data/haarcascade_frontalface_alt.xml", {}, function(err, faces) {
    if (err != null) {
      return cb(err);
    }
    return cb(null, faces);
  });
});

