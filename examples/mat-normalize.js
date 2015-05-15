var cv = require('../lib/opencv');




cv.readImage("./examples/files/mona.png", function(err, orig) {
  if (err) throw err;
  orig.convertGrayscale();

  var mask = new cv.Matrix(orig.height(),orig.width(),cv.Constants.CV_8UC1);
  var buf = Buffer(orig.height()*orig.width());
  buf.fill(0);
  for(var i=0;i<buf.length;i++){
    buf[i] = (i%2===1)?0:255;
  }
  mask.put(buf);
  orig.normalize(10,250,cv.Constants.NORM_TYPE_MASK,-1,mask);
  /*
  var window = new cv.NamedWindow('Mat-Normalize', 0);
  window.show(orig);
  window.blockingWaitKey(0, 50);
  setTimeout(function(){
    // keep the windows 30sec open
  },30000);
  */
});
