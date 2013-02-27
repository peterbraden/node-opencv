var Stream = require('stream').Stream
  , Buffers = require('buffers')
  , util = require('util')
  , path = require('path')

var bindings = require('./bindings')

var cv = module.exports = {};

cv.__proto__ = bindings;
/*

# Matrix #
The matrix is one of opencv's most core datatypes.

*/


var matrix = cv.Matrix.prototype;

matrix.detectObject = function(classifier, opts, cb){
	opts = opts || {}

  cv._detectObjectClassifiers = cv._detectObjectClassifiers || {}

  if (cv._detectObjectClassifiers[classifier]){
    var face_cascade = cv._detectObjectClassifiers[classifier];
  } else{
	  var face_cascade = new cv.CascadeClassifier(classifier);
    cv._detectObjectClassifiers[classifier] = face_cascade;
  }

	face_cascade.detectMultiScale(this, cb, opts.scale, opts.neighbors
		, opts.min && opts.min[0], opts.min && opts.min[1]);
}

matrix.inspect = function(){
	var size = this.size() ? (this.size()[0] + 'x' + this.size()[1]) : '';

	return "[Matrix " + size + " ]";

}

cv.ImageDataStream = function(){
	this.data = Buffers([])
	this.writable = true
}

util.inherits(cv.ImageDataStream, Stream);
var imagedatastream = cv.ImageDataStream.prototype;

imagedatastream.write = function(buf){
	this.data.push(buf)	
	return true;
}


imagedatastream.end = function(b){
	var self = this;
	
	if (b)
	  imagestream.write.call(this,b);

	var buf = this.data.toBuffer();

	cv.readImage(buf, function(err, im){
	  self.emit('load', im);
	});
}


<<<<<<< HEAD

cv.ImageStream = function(){
	this.writable = true
}

util.inherits(cv.ImageStream, Stream);
var imagestream = cv.ImageStream.prototype;

imagestream.write = function(buf){
	var self = this;
	cv.readImage(buf, function(err, matrix){
	  self.emit('data', matrix);
	});
}

// Object detect stream
cv.ObjectDetectionStream = function(cascade, opts){
  this.classifier = new cv.CascadeClassifier(cascade);
  this.opts = opts
}

util.inherits(cv.ObjectDetectionStream, Stream);
var ods = cv.ObjectDetectionStream.prototype;

ods.write = function(m){
  var self = this;

	this.classifier.detectMultiScale(m, 
      function(e, objs){
        if (e) { throw e }
        self.emit('data', objs, m);
      }
    , this.opts.scale, this.opts.neighbors
	  , this.opts.min && this.opts.min[0], this.opts.min && this.opts.min[1]);
}



// Provide cascade data for faces etc.

cv.FACE_CASCADE = path.resolve('./data/haarcascade_frontalface_alt.xml')


