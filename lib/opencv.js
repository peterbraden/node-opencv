var EventEmitter = require('events').EventEmitter

var opencv = require('./bindings')

module.exports = opencv;


/*

# Matrix #
The matrix is one of opencv's most core datatypes.

*/
opencv.Matrix.prototype.__proto__ = EventEmitter.prototype;





/*

# Image #

OpenCV has got rid of, so maybe we just use Matrix class
and augment in js:

    new cv.Image()

does

    cv.imageRead() // returns Matrix


and 
	
	Matrix.save()

does 

	cv.imwrite
	
*/