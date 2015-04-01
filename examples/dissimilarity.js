var cv = require('../lib/opencv');

cv.readImage("./examples/files/car1.jpg", function(err, car1) {
  if (err) throw err;

  cv.readImage("./examples/files/car2.jpg", function(err, car2) {
    if (err) throw err;

    cv.ImageSimilarity(car1, car2, function (err, dissimilarity) {
      if (err) throw err;

      console.log('Dissimilarity: ', dissimilarity);
    });

  });

});
