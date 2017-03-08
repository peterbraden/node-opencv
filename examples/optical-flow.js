var cv = require('../lib/opencv');

try {
  var camera = new cv.VideoCapture(0);
  var window = new cv.NamedWindow('Video', 0)
  /*
  setInterval(function() {
    camera.read(function(err, im) {
      if (err) throw err;
      console.log(im.size())
      if (im.size()[0] > 0 && im.size()[1] > 0){
        window.show(im);
      }
      window.blockingWaitKey(0, 50);
    });
  }, 20);
  */
} catch (e){
  console.log("Couldn't start camera:", e)
}
