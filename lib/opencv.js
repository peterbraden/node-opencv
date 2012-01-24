var EventEmitter = require('events').EventEmitter

var opencv = require('./bindings')

module.exports = opencv;


/*

# Matrix #
The matrix is one of opencv's most core datatypes.

*/
var matrix = opencv.Matrix.prototype;

matrix.__proto__ = EventEmitter.prototype;

matrix.faceDetect = function(classifier, opts, cb){
	var face_cascade = new opencv.CascadeClassifier("./examples/haarcascade_frontalface_alt.xml");
	face_cascade.detectMultiScale(this, cb);
}

/*

# Image #
	
*/