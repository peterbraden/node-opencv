var Stream = require('stream').Stream
  , Buffers = require('buffers')
  , util = require('util')
  , path = require('path');

var cv = module.exports = require('./bindings');

var Matrix = cv.Matrix
  , VideoCapture = cv.VideoCapture
  , ImageStream
  , ImageDataStream
  , ObjectDetectionStream
  , VideoStream;


Matrix.prototype.detectObject = function(classifier, opts, cb){
  var face_cascade;
  opts = opts || {};
  cv._detectObjectClassifiers = cv._detectObjectClassifiers || {};

  if (!(face_cascade = cv._detectObjectClassifiers[classifier])){
    face_cascade = new cv.CascadeClassifier(classifier);
    cv._detectObjectClassifiers[classifier] = face_cascade;
  }

  face_cascade.detectMultiScale(this, cb, opts.scale, opts.neighbors
    , opts.min && opts.min[0], opts.min && opts.min[1]);
}


Matrix.prototype.inspect = function(){
  var size = (this.size()||[]).join('x');
  return "[ Matrix " + size + " ]";
}


ImageStream = cv.ImageStream = function(){
  this.writable = true;
  this.tailPromise = Promise.resolve();
}
util.inherits(ImageStream, Stream);


ImageStream.prototype.end = function(){
  var self = this;
  this.tailPromise.then(function() {
    self.emit('finish');
  });
};

ImageStream.prototype.write = function(buf){
  var self = this;
  var prom = new Promise(function(resolve, reject) {
    cv.readImage(buf, function(err, matrix){
      if (err) {
        process.nextTick(function () {
          reject(err)
        });
        return self.emit('error', err);
      }
      self.emit('data', matrix);
      process.nextTick(resolve);
    });
  });
  // new tail promise
  this.tailPromise = this.tailPromise.then(function() {
    return prom;
  });
}

ImageDataStream = cv.ImageDataStream = function(){
  this.data = Buffers([]);
  this.writable = true;
}
util.inherits(ImageDataStream, Stream);


ImageDataStream.prototype.write = function(buf){
  this.data.push(buf);
  return true;
}


ImageDataStream.prototype.end = function(b){
  var self = this;
  if (b) ImageStream.prototype.write.call(this, b);

  var buf = this.data.toBuffer();
  cv.readImage(buf, function(err, im){
    if (err) return self.emit('error', err);
    self.emit('load', im);
  });
}


ObjectDetectionStream = cv.ObjectDetectionStream = function(cascade, opts){
  this.classifier = new cv.CascadeClassifier(cascade);
  this.opts = opts || {};
  this.readable = true;
  this.writable = true;
}
util.inherits(ObjectDetectionStream, Stream);


ObjectDetectionStream.prototype.write = function(m){
  var self = this;
  this.classifier.detectMultiScale(m, function(err, objs){
    if (err) return self.emit('error', err);
    self.emit('data', objs, m);
  }
  , this.opts.scale
  , this.opts.neighbors
  , this.opts.min && this.opts.min[0]
  , this.opts.min && this.opts.min[1]);
}


VideoStream = cv.VideoStream = function(src){
  if (!(src instanceof VideoCapture)) src = new VideoCapture(src);
  this.video = src;
  this.readable = true;
  this.paused = false;
}
util.inherits(VideoStream, Stream);


VideoStream.prototype.read = function(){
  var self = this;
  var frame = function(){
    self.video.read(function(err, mat){
      if (err) return self.emit('error', err);
      self.emit('data', mat);
      if (!self.paused) process.nextTick(frame);
    })
  }

  frame();
}


VideoStream.prototype.pause = function(){
  this.paused = true;
}


VideoStream.prototype.resume = function(){
  this.paused = false;
  this.read();
}


VideoCapture.prototype.toStream = function(){
  return new VideoStream(this);
}



// Provide cascade data for faces etc.
var CASCADES = {
  FACE_CASCADE: 'haarcascade_frontalface_alt.xml'
, EYE_CASCADE: 'haarcascade_eye.xml'
, EYEGLASSES_CASCADE: 'haarcascade_eye_tree_eyeglasses.xml'
, FULLBODY_CASCADE: 'haarcascade_fullbody.xml'
, CAR_SIDE_CASCADE: 'hogcascade_cars_sideview.xml'
}

Object.keys(CASCADES).forEach(function(k){
  cv[k] = path.resolve(__dirname, '../data', CASCADES[k])
})



