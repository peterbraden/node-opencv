var cv = require('../lib/opencv');

cv.readImage("./examples/files/mona.png", function(err, orig) {
  if (err) throw err;

  cv.readImage("./examples/over_text.png", function(err, over_text) {
    if (err) throw err;

    var result = new cv.Matrix(orig.width(), orig.height());
    result.addWeighted(orig, 0.7, over_text, 0.9);
    result.save("./tmp/weighted.png");
    console.log('Image saved to ./tmp/weighted.png');
  });
});
