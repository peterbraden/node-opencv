var cv = require('../lib/opencv')

var vid = new cv.VideoCapture(0)


var snap = function(){
  vid.read(function(im){
    im.detectObject(cv.FACE_CASCADE, {}, function(err, faces){
      if (!faces){
        console.log("No Faces")
        return;
      }
      var face = faces[0]
        , ims = im.size()
      
      im.adjustROI(
           -face.y
         , (face.y + face.height) - ims[0]
         , -face.x
         , (face.x + face.width) - ims[1])
   
      console.log("!!", im.locateROI());
      var im2 = im.clone()
    })


  });
}
snap()
