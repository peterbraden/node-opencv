var cv = require('../lib/opencv')

var win = new cv.NamedWindow("foo");
cv.readImage('./examples/stuff.png', function(e, im){
  win.show(im);
  setTimeout(function(){
    win.destroy();
  }, 1000)
})



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



cv.readImage("./examples/stuff.png", function(err, im){
  var i2 = im.copy()
  i2.convertGrayscale()
  i2.canny(5, 300)
  var features = i2.houghLinesP();
  for (var i=0;i<features.length; i++){
      var x = features[i]
      im.line([x[0], x[1]], [x[2], x[3]]);
      im.ellipse(x[0], x[1]);
      im.ellipse(x[2], x[3]);
    }
  console.log(features)
    im.save('./out.jpg');   
});
*/
