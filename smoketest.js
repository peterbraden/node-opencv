var opencv = require('./lib/opencv')
  , assert = require('assert')

assert.ok(!!opencv)
assert.ok(!!opencv.Point)
assert.ok(!!new opencv.Point(1, 2))
assert.equal(new opencv.Point(1, 2).x, 1)
assert.equal(new opencv.Point(1, 2).y, 2)
assert.equal(Math.round(new opencv.Point(1.1, 2).x * 100), 110)
assert.equal(Math.round(new opencv.Point(1.2, 2.75).y *100), 275)



console.log(opencv.Image("./examples/mona.jpg"))