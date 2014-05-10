var cv = require('../lib/opencv');

cv.readImage("./mona.png", function(err, im) {

  var srcArray = [
    0,0,
    im.width(),0,
    im.width(),im.height(),
    0,im.height()];
  var dstArray = [
    0,0,
    im.width()*.9,im.height()*.1,
    im.width(),im.height(),
    im.width()*.2,im.height()*.8];

  var xfrmMat = im.getPerspectiveTransform(srcArray,dstArray);
  img_warp = im.copy();
  img_warp.warpPerspective(xfrmMat,im.width,im.height,[255,255,255]);

  img_warp.save("/tmp/mona_warp.png");

});

