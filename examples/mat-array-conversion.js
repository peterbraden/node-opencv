var cv = require('../lib/opencv');

cv.readImage("./files/mona.png", function(err, orig) {
  if (err) throw err;

  var a = orig.toArray();
  var type = orig.type();
  var doubleConversion = cv.Matrix.fromArray(a, type).toArray();

  for(var i = 0 ; i < a.length; i++){
    for(var j = 0 ; j < a[i].length; j++){
      for(var k = 0 ; k < a[i][j].length; k++){
        if(a[i][j][k] !== doubleConversion[i][j][k]){
          throw(new Error("double conversion is not equal to original"));
        }
      }
    }
  }

});
