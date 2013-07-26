var cv = require('../lib/opencv')

var vid = new cv.VideoCapture(0)


var snap = function(){
  vid.read(function(err, im){
    im.detectObject(cv.FACE_CASCADE, {}, function(err, faces){

      if (!faces){
        console.log("No Faces")
        return;
      }
      var face = faces[0]
        , ims = im.size()
      
      var im2 = im.roi(face.x, face.y, face.width, face.height)
      /*
      im.adjustROI(
           -face.y
         , (face.y + face.height) - ims[0]
         , -face.x
         , (face.x + face.width) - ims[1])
         */
      im2.save('out.jpg')
    })


  });
}
snap()
