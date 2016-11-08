var cv = require('../lib/opencv');

cv.readImage("./files/mona.png", function(err, orig) {
  if (err) throw err;

  var chan1 = orig.split()[0];

  var floatMatrix = new cv.Matrix();
  var chan1Back = new cv.Matrix();

  chan1.convertTo(floatMatrix, cv.Constants.CV_32F);
  floatMatrix.convertTo(chan1Back, cv.Constants.CV_8U);

  var rowId = 0;
  var colId = 0;

  console.log(orig.get(rowId,colId), orig.pixel(rowId, colId));
  // ACTUAL : => -8.425768939900947e-273 [25, 51, 57]
  // EXPECTED : => [25, 51, 57] (this is R : 57 G : 51 and B : 25)
  console.log(chan1.get(rowId,colId), chan1.pixel(rowId, colId));
  // ACTUAL => 1.76244473776441e-105 36
  // EXPECTED => 154
  console.log(floatMatrix.get(rowId,colId), floatMatrix.pixel(rowId, colId));
  // ACTUAL => 481036405168
  // EXPECTED => 154.0
  console.log(chan1Back.get(rowId,colId), chan1Back.pixel(rowId, colId));
  // ACTUAL => 1.76244473776441e-105
  // EXPECTED => 154

});
