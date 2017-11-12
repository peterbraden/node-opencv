var cv = require('../lib/opencv');

cv.readImage('./files/mona.png', function(err, im) {
  if (err) throw err;
  if (im.width() < 1 || im.height() < 1) throw new Error('Image has no size');

  var refcount = im.getrefCount();
  console.log('initial refcount '+refcount);
  
  im.addref();
  var refcount2 = im.getrefCount();
  console.log('refcount after addref '+refcount2);
  if (refcount2 !== (refcount + 1))
	  throw "refcountmismatch";

  im.addref();
  refcount2 = im.getrefCount();
  console.log('refcount after addref 2 '+refcount2);
  if (refcount2 !== (refcount + 2))
	  throw "refcountmismatch";
	
	
  im.release();
  var refcount3 = im.getrefCount();
  console.log('refcount after release (seems should be -1) '+refcount3);
  if (refcount3 !== -1)
	  throw "refcountmismatch";

  // data is now still there somewhere, but lost to us - this is NOT a good situation.

  // should not fail - but will do absolutely nothing
  im.release();
  
  console.log('did all refcount tests');
});
