var cv = require('./lib/opencv')
/*
new cv.VideoCapture(0).read(function(mat){

  mat.resize(200,100)
  mat.save('./out.jpg')

  mat.detectObject("./data/haarcascade_frontalface_alt.xml", {min : [30,30]}, function(err, faces){  
    for (var i=0;i<faces.length; i++){
      var x = faces[i]
      mat.ellipse(x.x + x.width/2, x.y + x.height/2, x.width/2, x.height/2);
    }
    console.log(faces.length ? (faces.length + " faces found") : "No faces")
    mat.save('./out.jpg');   
    
  })
})
*/


cv.readImage("./examples/mona.png", function(err, im){
  var features = im.goodFeaturesToTrack();
  for (var i=0;i<features.length; i++){
      var x = features[i]
      im.ellipse(x[0] - 5, x[1] -5, 10, 10);
    }
  console.log(features)
    im.save('./out.jpg');   
});
