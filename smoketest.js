var cv = require('./lib/opencv')
  , assert = require('assert')

console.log(cv.version)


var im = new cv.Image("./examples/mona.jpg")
, face_cascade = new cv.CascadeClassifier("./examples/haarcascade_frontalface_alt.xml")

var faces = face_cascade.detectMultiScale(im, 1.1, 2, [30, 30])

for (var i=0;i<faces.length; i++){
	var x = faces[i]
	im.ellipse(x.x + x.width/2, x.y + x.height/2, x.width/2, x.height/2);
}
im.save('./out.jpg');
