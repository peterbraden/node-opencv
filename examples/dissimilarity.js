var cv = require('../lib/opencv');

if (cv.ImageSimilarity === undefined) {
  console.log('TODO: Please port Features2d.cc to OpenCV 3')
  process.exit(0);
}

cv.readImage("./files/car1.jpg", function(err, car1) {
  if (err) throw err;

  cv.readImage("./files/car2.jpg", function(err, car2) {
    if (err) throw err;

    cv.ImageSimilarity(car1, car2, function (err, dissimilarity) {
      if (err) throw err;

      console.log('Dissimilarity: ', dissimilarity);
    });

  });

});
