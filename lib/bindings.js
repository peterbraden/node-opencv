var binary = require('node-pre-gyp');
var path = require('path');
var binding_path = binary.find(path.resolve(path.join(__dirname,'../package.json')), { debug: !!process.env.NODE_OPENCV_DEBUG });
var binding = require(binding_path);

// module.exports = require('../build/Release/opencv.node');
module.exports = binding;
