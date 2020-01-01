var Stream = require('stream').Stream
  , Buffers = require('buffers')
  , util = require('util')
  , path = require('path')
  , os = require('os');

var cv = module.exports = require('./bindings');

var Matrix = cv.Matrix
  , ImageStream
  , ImageDataStream
  , ObjectDetectionStream
  , VideoStream;


if (cv.CascadeClassifier) {
  Matrix.prototype.detectObject = function (classifier, opts, cb) {
    var face_cascade;
    opts = opts || {};
    cv._detectObjectClassifiers = cv._detectObjectClassifiers || {};

    if (!(face_cascade = cv._detectObjectClassifiers[classifier])) {
      face_cascade = new cv.CascadeClassifier(classifier);
      cv._detectObjectClassifiers[classifier] = face_cascade;
    }

    face_cascade.detectMultiScale(this, cb, opts.scale, opts.neighbors
      , opts.min && opts.min[0], opts.min && opts.min[1]);
  }
} else {
  cv.Matrix.prototype.detectObject = function () {
    throw new Error("You need to install OpenCV with OBJDETECT module");
  }
}


cv.Matrix.prototype.inspect = function () {
  var size = (this.size() || []).join('x');
  return "[ Matrix " + size + " ]";
}

// we use the Opencv constants naming convention to extract the number of bytes (8, 16, 32, 64), and the number of channels from constants names
var getNumberOfBytesAndChannelsPerType = function (type) {
  var regExp = /CV_([0-9]+)([A-Z]+)([0-9]+)/;
  for (var k in cv.Constants) if (cv.Constants.hasOwnProperty(k) && k.match(regExp) && cv.Constants[k] === type) {
    var bytes, channels, dataType;
    k.replace(regExp, function (all, b, l, c) {
      bytes = b;
      channels = c;
      dataType = l[0]
    });

    return {
      bytes: parseInt(bytes),
      channels: !isNaN(parseInt(channels)) && parseInt(channels),
      dataType: dataType,
      label: k
    };
  }
};

var getBufferMethodName = function (bytes, dataType, endianness, read) {
  var fnName;

  if (read) {
    fnName = "read";
  } else {
    fnName = "write";
  }

  if (bytes === 32 && (dataType === "F" || dataType === "S")) {
    if (dataType === "F") {
      fnName += "Float" + endianness;
    } else {//dataType === "S"
      fnName += "Int32" + endianness;
    }
  } else if (bytes === 8) {
    fnName += (dataType === "U" ? "U" : "") + "Int8";
  } else if (bytes === 16) {
    fnName += (dataType === "U" ? "U" : "") + "Int16" + endianness;
  } else {
    throw ("This matrix type (CV_" + bytes + dataType + ") is not compatible with fromArray/toArray")
  }
  return fnName;
};

Matrix.fromArray = function (arr, type) {
  var n_bytes;

  var bytesAndChannels = getNumberOfBytesAndChannelsPerType(type);
  var bytes = bytesAndChannels.bytes;
  var channels = bytesAndChannels.channels;
  var dataType = bytesAndChannels.dataType;
  var label = bytesAndChannels.label;

  if (!Array.isArray(arr) || !Array.isArray(arr[0]) || !Array.isArray(arr[0][0]) || (channels && arr[0][0].length !== channels)) {
    throw (new Error("Input array must be a 3-level array/matrix with size rows x cols x channels corresponding to dataType (" + label + ")"));
  }

  if (!channels) {
    channels = 1;
  }

  var rows = arr.length;
  var cols = arr[0].length;

  var mat = new cv.Matrix(rows, cols, type);

  var n_bytes = bytes / 8;
  var buf = new Buffer(rows * cols * channels * n_bytes);

  buf.fill(0);

  var fnName = getBufferMethodName(bytes, dataType, os.endianness(), false)

  for (var i = 0; i < rows * cols * channels; i++) {
    var c = i % channels;
    var r = Math.floor(i / channels);
    var y = r % cols;
    var x = Math.floor(r / cols);
    buf[fnName](arr[x][y][c], i * n_bytes);
  }

  mat.put(buf);

  return mat;
}

Matrix.prototype.toArray = function () {
  var size = this.size();
  var buf = this.getData();
  var type = this.type();
  var bytesAndChannels = getNumberOfBytesAndChannelsPerType(type);
  var bytes = bytesAndChannels.bytes;
  var channels = bytesAndChannels.channels || this.channels();
  var dataType = bytesAndChannels.dataType;

  var n_bytes = bytes / 8;
  var fnName = getBufferMethodName(bytes, dataType, os.endianness(), true)

  var res = []
  for (var i = 0; i < size[0]; i++) {
    var row = [];
    for (var j = 0; j < size[1]; j++) {
      var channelsValues = [];
      for (var k = 0; k < channels; k++) {
        var index = (i * size[1] + j) * channels + k;
        channelsValues.push(buf[fnName](index * n_bytes));
      }
      row.push(channelsValues);
    }
    res.push(row);
  }
  return res;
}

ImageStream = cv.ImageStream = function () {
  this.writable = true;
}
util.inherits(ImageStream, Stream);


ImageStream.prototype.write = function (buf) {
  var self = this;
  cv.readImage(buf, function (err, matrix) {
    if (err) return self.emit('error', err);
    self.emit('data', matrix);
  });
}


ImageDataStream = cv.ImageDataStream = function () {
  this.data = Buffers([]);
  this.writable = true;
}
util.inherits(ImageDataStream, Stream);


ImageDataStream.prototype.write = function (buf) {
  this.data.push(buf);
  return true;
}


ImageDataStream.prototype.end = function (b) {
  var self = this;
  if (b) ImageStream.prototype.write.call(this, b);

  var buf = this.data.toBuffer();
  cv.readImage(buf, function (err, im) {
    if (err) return self.emit('error', err);
    self.emit('load', im);
  });
}

if (cv.CascadeClassifier) {
  ObjectDetectionStream = cv.ObjectDetectionStream = function (cascade, opts) {
    this.classifier = new cv.CascadeClassifier(cascade);
    this.opts = opts || {};
    this.readable = true;
    this.writable = true;
  }
  util.inherits(ObjectDetectionStream, Stream);

  ObjectDetectionStream.prototype.write = function (m) {
    var self = this;
    this.classifier.detectMultiScale(m, function (err, objs) {
      if (err) return self.emit('error', err);
      self.emit('data', objs, m);
    }
      , this.opts.scale
      , this.opts.neighbors
      , this.opts.min && this.opts.min[0]
      , this.opts.min && this.opts.min[1]);
  }
}


if (cv.VideoCapture) {
  var VideoCapture = cv.VideoCapture

  VideoStream = cv.VideoStream = function (src) {
    if (!(src instanceof VideoCapture)) src = new VideoCapture(src);
    this.video = src;
    this.readable = true;
    this.paused = false;
  }
  util.inherits(VideoStream, Stream);


  VideoStream.prototype.read = function () {
    var self = this;
    var frame = function () {
      self.video.read(function (err, mat) {
        if (err) return self.emit('error', err);
        self.emit('data', mat);
        if (!self.paused) process.nextTick(frame);
      })
    }

    frame();
  }

  VideoStream.prototype.pause = function () {
    this.paused = true;
  }


  VideoStream.prototype.resume = function () {
    this.paused = false;
    this.read();
  }

  VideoCapture.prototype.toStream = function () {
    return new VideoStream(this);
  }
}


// Provide cascade data for faces etc.
var CASCADES = {
  FACE_CASCADE: 'haarcascade_frontalface_alt.xml'
  , EYE_CASCADE: 'haarcascade_eye.xml'
  , EYEGLASSES_CASCADE: 'haarcascade_eye_tree_eyeglasses.xml'
  , FULLBODY_CASCADE: 'haarcascade_fullbody.xml'
  , CAR_SIDE_CASCADE: 'hogcascade_cars_sideview.xml'
  , ANIME_FACE_CASCADE: 'lbpcascade_animeface.xml'
}

Object.keys(CASCADES).forEach(function (k) {
  cv[k] = path.resolve(__dirname, '../data', CASCADES[k])
})
