var cv = require('../lib/opencv');

cv.readImage('./files/mona.png', function(err, im) {
  if (err) throw err;
  if (im.width() < 1 || im.height() < 1) throw new Error('Image has no size');


  // getrefCount will give the refcount.  if > 0, then a reference ptr is present, and the value returned.
  // if -1, no reference pointer was present (i.e. the mat does not know about any data).
  var refcount = im.getrefCount();
  console.log('initial refcount '+refcount);
  if (refcount !== 1)
	  throw "refcountmismatch - initial should be 1";
  
  im.release();
  var refcount3 = im.getrefCount();
  console.log('refcount after release (seems should be -1) '+refcount3);
  if (refcount3 !== -1)
	  throw "refcountmismatch - after release should be -1";

  // data is now still there somewhere, but lost to us - this is NOT a good situation.

  // should not fail - but will do absolutely nothing
  im.release();
  
  console.log('did all refcount tests');
});
