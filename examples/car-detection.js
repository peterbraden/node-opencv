var cv = require('../lib/opencv');

cv.readImage("./examples/files/car1.jpg", function(err, im){
  if (err) throw err;
  if (im.width() < 1 || im.height() < 1) throw new Error('Image has no size');

  im.detectObject("./data/hogcascade_cars_sideview.xml", {}, function(err, cars){
    if (err) throw err;

    for (var i=0; i < cars.length; i++){
      var x = cars[i];
      im.rectangle([x.x, x.y], [x.width, x.height]);
    }

    im.save('./examples/tmp/car-detection.jpg');
    console.log('Image saved to ./examples/tmp/car-detection.jpg');
  });
});
