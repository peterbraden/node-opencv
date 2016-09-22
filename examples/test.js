var cv = require('../lib/opencv');

var mat = new cv.Matrix(1, 2, cv.Constants.CV_8UC3, [1]);
var row = mat.pixelRow(0);
console.log("mat: " + row[0] + row[1]);
