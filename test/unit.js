var fs = require('fs')
  , test = require('tape')
  , cv = null



test("Smoke tests / Can Import", function(t){
  cv = require('../lib/opencv')
  t.ok(cv, "imported fine")
  t.ok(cv.version, "version is there:" + cv.version)
  t.ok(cv.Point, "point is there")
  t.ok(cv.Matrix, "matrix is there")
  t.end()
})


test('importing library multiple times is ok', function(t){
  var cv1 = require('../lib/opencv')
    , cv2 = require('../lib/opencv')
    cv1.readImage('./examples/files/mona.png', function(err, im){
      t.error(err)
      cv2.readImage('./examples/files/mona.png', function(err, im){
        t.error(err)
        t.end();
      });
    });
})


test('Point', function(t){

  t.ok(new cv.Point(1, 2))
  t.throws(function () { cv.Point(1, 2)}, TypeError, "cannot call without new")

  t.equal(new cv.Point(1, 2).x, 1)
  t.equal(new cv.Point(1, 2).y, 2)
  t.equal(Math.round(new cv.Point(1.1, 2).x * 100), 110)
  t.equal(Math.round(new cv.Point(1.2, 2.75).y *100), 275)

  t.throws(function () {new cv.Point(1.1, 2).x = 5}, Error, "Points are immutable")
  t.throws(function () {new cv.Point(1.1, 2).y = 5}, Error, "Points are immutable")

  var p1 = new cv.Point(3, 6)
    , p2 = new cv.Point(5, 7)

  t.ok(p1.dot);
  t.equal(p1.dot(p2), 57);

  t.end()
})


test('Matrix constructor', function(assert){
  assert.ok(cv.Matrix);
  assert.ok(new cv.Matrix);
  assert.ok(new cv.Matrix(1,2));
  assert.end()
})

test('Matrix accessors', function(assert){
  var mat = new cv.Matrix(1, 2);
  mat.set(0,0,3)
  mat.set(0,1,5000)
  assert.deepEqual(mat.row(0), [3,5000]);

  mat = new cv.Matrix(1,2);
  assert.equal(mat.set(0,0,3), undefined);
  assert.equal(mat.get(0,0), 3);

  mat = new cv.Matrix(6,7);
  assert.equal(mat.width(), 7);

  mat = new cv.Matrix(6,7);
  assert.equal(mat.height(), 6);

  mat = new cv.Matrix(6,7);
  assert.deepEqual(mat.size(), [6, 7]);

  mat = new cv.Matrix(6,7);
  assert.equal(mat.width(), 7);
  mat.resize(8,9);
  assert.equal(mat.width(), 8);

  mat = new cv.Matrix.Eye(4,4)
  assert.deepEqual(mat.row(1), [0,1,0,0])
  assert.deepEqual(mat.row(2), [0,0,1,0])

  mat = new cv.Matrix.Eye(4,4);
  assert.deepEqual(mat.col(1), [0,1,0,0])
  assert.deepEqual(mat.col(2), [0,0,1,0])

  assert.equal(new cv.Matrix().empty(), true);

  assert.end()
})


test("Matrix toBuffer", function(assert){
  var buf = fs.readFileSync('./examples/files/mona.png')

  cv.readImage(buf.slice(0), function(err, mat){
    var buf0 = mat.toBuffer()
    assert.ok(buf0);
    assert.end()
  })
})



test("Matrix toBuffer Async", function(assert){
  var buf = fs.readFileSync('./examples/files/mona.png')

  cv.readImage(buf.slice(0), function(err, mat){
    mat.toBuffer(function(err, buff){
      assert.error(err)
      assert.ok(buf)
      assert.ok(buf.length > 100)

      assert.end()
    })
  })
})


test("detectObject", function(assert){
  cv.readImage("./examples/files/mona.png", function(err, im){
    im.detectObject(cv.FACE_CASCADE, {}, function(err, faces){
      assert.error(err)
      assert.ok(faces)
      assert.equal(faces.length, 1)
      assert.end()
    })
  })
})

test(".absDiff and .countNonZero", function(assert){
  cv.readImage("./examples/files/mona.png", function(err, im) {
    cv.readImage("./examples/files/mona.png", function(err, im2){
      assert.ok(im);
      assert.ok(im2);

      var diff = new cv.Matrix(im.width(), im.height());
      diff.absDiff(im, im2);

      diff.convertGrayscale();
      assert.equal(diff.countNonZero(), 0);
      assert.end()
    });
  });
})


test(".bitwiseXor", function(assert){
  var mat1 = new cv.Matrix(1,1);
  mat1.set(0,0, 1);

  var mat2 = new cv.Matrix(1,1);
  mat2.set(0,0, 1);

  var xored = new cv.Matrix(1,1);
  xored.bitwiseXor(mat1, mat2);

  assert.equal(xored.get(0,0), 0);

  assert.end()
})


test("Image read from file", function(assert){
  cv.readImage("./examples/files/mona.png", function(err, im){
    assert.ok(im);
    assert.equal(im.width(), 500);
    assert.equal(im.height(), 756)
    assert.equal(im.empty(), false)
    assert.end()
  })
})


test("read Image from buffer", function(assert){
  cv.readImage(fs.readFileSync('./examples/files/mona.png'), function(err, im){
    assert.ok(im);
    assert.equal(im.width(), 500);
    assert.equal(im.height(), 756)
    assert.equal(im.empty(), false)
    assert.end()
  })
})

test("Cascade Classifier", function(assert){
  assert.ok(new cv.CascadeClassifier("./data/haarcascade_frontalface_alt.xml"), 'test constructor')

  cv.readImage("./examples/files/mona.png", function(err, im){
    cascade = new cv.CascadeClassifier("./data/haarcascade_frontalface_alt.xml");
    cascade.detectMultiScale(im, function(err, faces){//, 1.1, 2, [30, 30]);
      assert.error(err);
      assert.equal(typeof faces, typeof []);
      assert.equal(faces.length, 1)
      assert.end()
    })
  })
})


test("ImageDataStream", function(assert){
  var s = new cv.ImageDataStream()
  s.on('load', function(im){ 
    assert.ok(im)
    assert.equal(im.empty(), false);
    assert.end()
  })

  fs.createReadStream('./examples/files/mona.png').pipe(s);

})

test("ImageStream", function(assert){
  var s = new cv.ImageStream()
    , im = fs.readFileSync('./examples/files/mona.png')

  s.on('data', function(mat){
    assert.deepEqual(mat.size(), [756,500])
    assert.end()
  })
  s.write(im);
})


test("CamShift", function(assert){
  cv.readImage('./examples/files/coin1.jpg', function(e, im){
    cv.readImage('./examples/files/coin2.jpg', function(e, im2){
      var tracked = new cv.TrackedObject(im, [420, 110, 490, 170], {channel: 'v'});
      assert.ok(tracked);
      var res = tracked.track(im2)
      assert.ok(res);
      assert.ok(res[0]  < 396)
      assert.ok(res[0]  > 376)
      assert.ok(res[1]  < 122)
      assert.ok(res[1]  > 102)
      assert.ok(res[2]  < 469)
      assert.ok(res[2]  > 449)
      assert.ok(res[3]  < 176)
      assert.ok(res[3]  > 156)
      assert.end()
    })
  })
})

test("fonts", function(t) {

  function rnd() {
    return Math.round(Math.random() * 255);
  };

  cv.readImage('./examples/files/coin1.jpg', function(e, im){
    var y = 0;

    ([
      "HERSEY_SIMPLEX",
      "HERSEY_PLAIN",
      "HERSEY_DUPLEX",
      "HERSEY_COMPLEX",
      "HERSEY_TRIPLEX",
      "HERSEY_COMPLEX_SMALL",
      "HERSEY_SCRIPT_SIMPLEX",
      "HERSEY_SCRIPT_COMPLEX",
      "HERSEY_SCRIPT_SIMPLEX"
    ]).forEach(function(font) {
      im.putText("Some text", 0, y += 20, font, [rnd(), rnd(), rnd()]);
    });

    t.ok(im, "image is ok")
    //im.save("./examples/tmp/coin1-with-text.jpg");
    t.end();
  });
})

