module.exports = require('../build/Release/opencv.node');


try {
  module.exports = require('../build/Release/opencv.node');
} catch (e) { try {
  module.exports = require('../build/default/opencv.node');
} catch (e) {
  throw e;
}}