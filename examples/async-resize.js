var cv = require('../lib/opencv');

cv.readImage("./files/mona.png", function(err, im) {
  if (err) throw err;

  var width = im.width();
  var height = im.height();
  if (width < 1 || height < 1) throw new Error('Image has no size');
  
  console.log('Image loaded from ./files/mona.png at '+im.width()+'x'+im.height());
  
  var AfterResize = function(err, img){
    if (err){
      console.log('Error in resize:' + err);
      return;
    }
    img.save("./tmp/resize-async-image.png");
    console.log('Image saved to ./tmp/resize-async-image.png at '+img.width()+'x'+img.height());
  };

  var newwidth = width*0.95;
  var newheight = height*0.95;
  
  var Async = true;
  if (Async){
    // note - generates a new image
    im.resize(newwidth, newheight, AfterResize);
  } else {
    // sync - note - modifies the input image
    im.resize(newwidth, newheight);
    AfterResize(null, im);
  }
  
});
