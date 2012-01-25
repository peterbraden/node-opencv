var EventEmitter = require('events').EventEmitter
  , Buffers = require('buffers')

var bindings = require('./bindings')

var cv = module.exports = {};

cv.__proto__ = bindings;
console.log(cv)
/*

# Matrix #
The matrix is one of opencv's most core datatypes.

*/


var matrix = cv.Matrix.prototype;

matrix.faceDetect = function(classifier, opts, cb){
	var face_cascade = new opencv.CascadeClassifier("./examples/haarcascade_frontalface_alt.xml");
	face_cascade.detectMultiScale(this, cb);
}

/*

# Image #
	
*/


cv.ImageStream = function(){
	this.data = Buffers([])
}

var imagestream = cv.ImageStream.prototype;
imagestream.__proto__ = EventEmitter.prototype;

imagestream.on('data', function(buf){
	this.data.push(buf)	
	console.log("!!!<>");
})

imagestream.on('end', function(){
	var buf = this.data.toBuffer();
	//var im = opencv.readImage(buf);
	console.log(this.data.length, buf.length);
	//this.emit('onload', im);
})

