var cv = require('../lib/opencv');
var vid = new cv.VideoCapture(0);

vid.read(function(err, im){
  if (err) throw err;

  im.detectObject(cv.FACE_CASCADE, {}, function(err, faces){
    if (err) throw err;
    if (!faces.length) return console.log("No Faces");

    var face = faces[0];
    var ims = im.size();
    var im2 = im.roi(face.x, face.y, face.width, face.height)
    /*
    im.adjustROI(
         -face.y
       , (face.y + face.height) - ims[0]
       , -face.x
       , (face.x + face.width) - ims[1])
       */
    im2.save('./examples/tmp/take-face-pics.jpg')
    console.log('Image saved to ./tmp/take-face-pics.jpg');
  })
});
